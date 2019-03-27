#include "OKdio.h"
#include "Effector.h"
#include "XAudio2/XAudio2.h"
#include "XAudio2/VoiceCallback.h"
#include "Loader/Loader.h"
#include <ks.h>
#include <ksmedia.h>
#include <algorithm>

// スピーカー設定用配列
const unsigned long spk[] = {
	KSAUDIO_SPEAKER_MONO,
	KSAUDIO_SPEAKER_STEREO,
	KSAUDIO_SPEAKER_STEREO | SPEAKER_LOW_FREQUENCY,
	KSAUDIO_SPEAKER_QUAD,
	0,
	KSAUDIO_SPEAKER_5POINT1,
	0,
	KSAUDIO_SPEAKER_7POINT1_SURROUND
};

// バッファ最大数
#define BUFFER 2

// コンストラクタ
Okdio::Okdio()
{
	Init();
}

// コンストラクタ
Okdio::Okdio(Effector* effector)
{
	Init();

	this->effector = effector;
}

// コンストラクタ
Okdio::Okdio(const std::string& fileName)
{
	Init();
	Load(fileName);
}

// コンストラクタ
Okdio::Okdio(const snd::Info& info, const std::vector<float>& data)
{
	Init();
	CreateOriginal(info, data);
}

// コンストラクタ
Okdio::Okdio(const std::string& fileName, Effector* effector)
{
	Init();
	this->effector = effector;
	Load(fileName);
}

// コンストラクタ
Okdio::Okdio(const snd::Info& info, const std::vector<float>& data, Effector* effector)
{
	Init();
	this->effector = effector;
	CreateOriginal(info, data);
}

// コピーコンストラクタ
Okdio::Okdio(const Okdio& okdio)
{
	(*this) = okdio;
}

// デストラクタ
Okdio::~Okdio()
{
	if (voice != nullptr)
	{
		voice->DestroyVoice();
	}
}

// 初期化
void Okdio::Init(void)
{
	back     = std::make_unique<VoiceCallback>(this);
	effector = nullptr;
	handle   = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
	voice    = nullptr;
	loop     = false;
	cnt      = 0;
	index    = 0;
	
	read.assign(1, 0);
	data.resize(BUFFER);
}

// ソースボイス生成
long Okdio::CreateVoice(void)
{
	snd::Info info = Loader::Get().Info(name);

	WAVEFORMATEXTENSIBLE desc{};
	desc.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	desc.Format.nChannels       = info.channel;
	desc.Format.nSamplesPerSec  = info.sample;
	desc.Format.nBlockAlign     = sizeof(float) * desc.Format.nChannels;
	desc.Format.wBitsPerSample  = sizeof(float) * 8;
	desc.Format.nAvgBytesPerSec = desc.Format.nSamplesPerSec * desc.Format.nBlockAlign;
	desc.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;

	desc.dwChannelMask               = spk[desc.Format.nChannels - 1];
	desc.Samples.wValidBitsPerSample = desc.Format.wBitsPerSample;
	desc.SubFormat                   = KSDATAFORMAT_SUBTYPE_IEEE_FLOAT;
	
	auto hr = XAudio2::Get().Audio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&desc), 0, 1.0f, &(*back));
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugStringA("\nソースボイス生成：失敗\n");
	}
#endif

	return hr;
}

// ファイルから読み込み
int Okdio::Load(const std::string& fileName)
{
	if (Loader::Get().Load(fileName) != 0)
	{
		return -1;
	}

	name = fileName;

	if (FAILED(CreateVoice()))
	{
		return -1;
	}

	UpData();

	return 0;
}

// オリジナル波形生成
int Okdio::CreateOriginal(const snd::Info& info, const std::vector<float>& data)
{
	name = Loader::Get().Create(info, data);

	if (FAILED(CreateVoice()))
	{
		return -1;
	}

	UpData();

	return 0;
}

// エフェクトを末尾に追加
void Okdio::PushEffect(Effect* effect)
{
	this->effect.emplace_back(effect);
}

// エフェクトをまとめてセット
void Okdio::SetEffect(const std::initializer_list<Effect*>& effect)
{
	this->effect = effect;

	UpData();
}

// 再生
long Okdio::Play(const bool& loop, const size_t& overlaidMax)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugStringA("\n再生：失敗\n");
#endif
		return hr;
	}

	this->loop = loop;

	cnt += (cnt + 1 >= overlaidMax) ? 0 : 1;
	if (cnt > read.size())
	{
		read.push_back(0);
	}

	return hr;
}

// 停止
long Okdio::Stop(void)
{
	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugStringA("\n停止：失敗\n");
	}

	return hr;
}

// 波形データをボイスバッファに追加
long Okdio::Submit(void)
{
	XAUDIO2_BUFFER buf{};
	buf.AudioBytes = unsigned int(sizeof(float) * data[index].size());
	buf.pAudioData = (unsigned char*)(data[index].data());

	auto hr = voice->SubmitSourceBuffer(&buf);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugStringA("\nソースボイスのバッファ追加：失敗\n");
#endif
		return hr;
	}

	index = (index + 1 >= BUFFER) ? 0 : ++index;
	for (size_t& i : read)
	{
		i += Bps();
	}

	return hr;
}

// 波形データ更新
void Okdio::UpData(void)
{
	data[index].assign(Bps(), 0.0f);

	//データ全体サイズ
	const size_t dataSize = Loader::Get().Data(name).lock()->size();

	for (size_t& i : read)
	{
		//残りサイズ
		const size_t size = (dataSize - i >= Bps()) ? Bps() : dataSize - i;
		if (i + size >= dataSize)
		{
			continue;
		}

		std::transform(&Loader::Get().Data(name).lock()->at(i), &Loader::Get().Data(name).lock()->at(i + size), data[index].begin(), data[index].begin(), std::plus<float>());
	}

	//エフェクターに追加
	if (effector != nullptr)
	{
		effector->Push(this);
	}
}

// 終了確認
void Okdio::CheckEnd(void)
{
	//データ全体サイズ
	const size_t dataSize = Loader::Get().Data(name).lock()->size();

	for (std::vector<size_t>::iterator itr = read.begin(); itr != read.end();)
	{
		if ((*itr) >= dataSize)
		{
			if (loop == false)
			{
				itr = read.erase(itr);
				--cnt;
				continue;
			}

			(*itr) = 0;
		}

		++itr;
	}
}

// リセット
void Okdio::Reset(void)
{
	if (read.size() <= 0)
	{
		Stop();
		read.push_back(0);
	}
}

// 一回の処理データ取得
inline size_t Okdio::Bps(void) const
{
	snd::Info info = Loader::Get().Info(name);
	return info.sample * info.channel / 100;
}

// 現在の波形情報取得
std::vector<float>& Okdio::Data(void)
{
	return data[index];
}

// 代入演算子オーバーロード
void Okdio::operator=(const Okdio& okdio)
{
	Init();

	effector = okdio.effector;
	name     = okdio.name;
	if (FAILED(CreateVoice()))
	{
		return;
	}

	UpData();
}
