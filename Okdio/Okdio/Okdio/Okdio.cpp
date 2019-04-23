#include "Okdio.h"
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

// 波形処理範囲のオフセット
#define OFFSET 100

// コンストラクタ
Okdio::Okdio(Effector* effector) : 
	effector(effector)
{
	Init();
}

// コンストラクタ
Okdio::Okdio(const std::string& fileName, Effector* effector) : 
	effector(effector)
{
	Init();
	Load(fileName);
}

// コンストラクタ
Okdio::Okdio(const snd::Info& info, const std::vector<float>& data, Effector* effector) : 
	effector(effector)
{
	Init();
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
	if (effector != nullptr)
	{
		WaitForSingleObject(handle, INFINITE);
	}

	if (voice != nullptr)
	{
		voice->DestroyVoice();
	}

	CloseHandle(handle);
}

// 初期化
void Okdio::Init(void)
{
	back     = std::make_unique<VoiceCallback>(this);
	voice    = nullptr;
	handle   = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
	endFlag  = false;
	loop     = false;
	cnt      = 0;
	index    = 0;
	
	memset(&inout[0], 0, sizeof(inout));
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

	endFlag    = false;
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
		return hr;
	}

	return hr;
}

// 高速フーリエ変換
void FFT(void)
{
	//n　 入力信号配列サイズ
	//id　IDの並び
	auto calc = [&](const unsigned int& n, std::vector<unsigned int>& id)->unsigned int {
		//2の累乗の基数
		unsigned int stage = unsigned int(std::ceil(std::log2(n)));

		//配列メモリ確保
		id.reserve(n);

		id.push_back(0);
		id.push_back(1);
		for (unsigned int st = 0; st < stage - 1; ++st)
		{
			for (unsigned int& i : id)
			{
				i *= 2;
			}

			//追加
			id.insert(id.end(), id.begin(), id.end());

			//イテレータを配列サイズ分進める
			auto itr = id.begin();
			std::advance(itr, id.size() / 2);

			for (; itr != id.end(); ++itr)
			{
				(*itr) += 1;
			}
		}

		return stage;
	};

	//data　 入力データ
	//arg　　出力の実部、虚部
	auto fft = [&](const std::vector<std::complex<double>>& data, std::vector<std::complex<double>>& arg, const bool& inverse = false)->void {
		std::vector<unsigned int>id;
		unsigned int stage = calc(data.size(), id);

		arg.assign(data.size(), 0.0);
		for (unsigned int i = 0; i < data.size(); ++i)
		{
			arg[i] = data[id[i]];
		}

		unsigned int po2 = 1;

		for (unsigned int st = 1; st <= stage; ++st)
		{
			po2 <<= 1;
			const int po2m = po2 >> 1;

			auto w = std::exp(std::complex<double>(0.0, 2.0 * double(snd::PI()) / po2));
			w = (inverse == true) ? std::conj(w) : w;

			auto ws = std::complex<double>(1.0, 0.0);

			//バタフライ
			for (unsigned int i = 0; i < po2m; ++i)
			{
				for (unsigned int n = 0; n < data.size(); n += po2)
				{
					auto wfb = ws * arg[n + i + po2m];
					arg[n + i + po2m] = arg[n + i] - wfb;
					arg[n + i] += wfb;
				}

				ws *= w;
			}
		}
	};

	auto ifft = [&](const std::vector<std::complex<double>> & arg, std::vector<std::complex<double>> & data)->void {
		fft(arg, data, true);

		std::for_each(data.begin(), data.end(), [&](std::complex<double>& val)->void {
			val /= data.size();
		});
	};

	std::vector<std::complex<double>>data = {
		0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0
	};
	std::vector<std::complex<double>>arg;
	fft(data, arg);

	std::vector<std::complex<double>>tmp;
	ifft(arg, tmp);

	std::vector<std::complex<int>>tmp2(tmp.begin(), tmp.end());

	int qqq = 0;
}

// 波形データをボイスバッファに追加
long Okdio::Submit(void)
{
	if (effector != nullptr)
	{
		WaitForSingleObject(handle, INFINITE);
	}

	FFT();

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
		endFlag = true;
	}
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

// サウンド情報取得
snd::Info Okdio::GetInfo(void) const
{
	return Loader::Get().Info(name);
}

// 再生終了確認
bool Okdio::IsPlayEnd(void) const
{
	return endFlag;
}

// 参照ファイルパス取得
std::string Okdio::GetName(void) const
{
	return name;
}

// 現在の波形情報取得
std::vector<float>& Okdio::GetData(void)
{
	return data[index];
}

// フィルタ用入出力データ取得
float* Okdio::GetInOut(void)
{
	return inout;
}

// 一回の処理データ取得
inline size_t Okdio::Bps(void) const
{
	snd::Info info = Loader::Get().Info(name);
	return info.sample * info.channel / OFFSET;
}
