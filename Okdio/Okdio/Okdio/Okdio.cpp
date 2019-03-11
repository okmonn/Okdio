#include "Okdio.h"
#include "XAudio2/XAudio2.h"
#include "SoundLoader/SoundLoader.h"
#include "Effector/Effector.h"
#include <ks.h>
#include <ksmedia.h>
#include <algorithm>
#include <tchar.h>

// メモリ解放
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }
// オフセット
#define OFFSET 100
// バッファ数
#define BUFFER 2
// 最大多重可能数
#define CNT_MAX 10

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

// コンストラクタ
Okdio::Okdio()
{
	Init();
}

// コンストラクタ
Okdio::Okdio(const std::string& fileName)
{
	Init();
	Load(fileName);
}

// コピーコンストラクタ
Okdio::Okdio(const Okdio& okdio)
{
	*this = okdio;
}

// デストラクタ
Okdio::~Okdio()
{
	Destroy(voice);
	CloseHandle(handle);
}

// 初期化
void Okdio::Init(void)
{
	voice  = nullptr;
	name   = std::nullopt;
	index  = 0;
	cnt    = 1;
	loop   = false;
	handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

	read.push_back(0);
	wave.resize(BUFFER);
}

// ソースボイス生成
long Okdio::CreateVoice(void)
{
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

	auto hr = XAudio2::Get().Audio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)(&desc), 0, 1.0f, this);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nボイス生成：失敗\n"));
	}
#endif

	return hr;
}

// 波形更新
void Okdio::UpData(void)
{
	wave[index].assign(Bps(), 0.0f);
	for (unsigned int& i : read)
	{
		//残りサイズ計算
		unsigned int size = (unsigned int(SoundLoader::Get().Wave(name.value())->size()) - i > Bps())
			? Bps()
			: unsigned int(SoundLoader::Get().Wave(name.value())->size()) - i - 1;

		if (i + size >= SoundLoader::Get().Wave(name.value())->size())
		{
			continue;
		}

		std::transform(&SoundLoader::Get().Wave(name.value())->at(i), &SoundLoader::Get().Wave(name.value())->at(i + size), wave[index].begin(), wave[index].begin(), std::plus<float>());
	}

	Effector::Get().Execution(this);
}

// 読み込み
int Okdio::Load(const std::string& fileName)
{
	if (SoundLoader::Get().Load(fileName, info) != 0)
	{
		return -1;
	}

	if (FAILED(CreateVoice()))
	{
		return -1;
	}

	name = fileName;

	UpData();

	return 0;
}

// 再生
long Okdio::Play(const bool& loop, const bool& multiple)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugString(_T("\n再生：失敗\n"));
		return hr;
#endif
	}

	this->loop = loop;

	if (multiple)
	{
		cnt = (cnt + 1 > CNT_MAX) ? cnt : ++cnt;
		if (read.size() < cnt)
		{
			read.push_back(0);
		}
	}

	return hr;
}

// 停止
long Okdio::Stop(void)
{
	auto hr = voice->Stop();
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n停止：失敗\n"));
	}
#endif

	return hr;
}

// バッファに追加
long Okdio::Submit(void)
{
	//バッファに追加
	XAUDIO2_BUFFER buf{};
	buf.AudioBytes = unsigned int(sizeof(float) * wave[index].size());
	buf.pAudioData = (unsigned char*)(wave[index].data());

	auto hr = voice->SubmitSourceBuffer(&buf);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugString(_T("\nバッファに追加：失敗\n"));
#endif
		return hr;
	}
	
	index = (index + 1 >= BUFFER) ? 0 : ++index;

	for (unsigned int& i : read)
	{
		i += Bps();
	}

	return hr;
}

// 終了確認
void Okdio::CheckEnd(void)
{
	for (auto itr = read.begin(); itr != read.end();)
	{
		if ((*itr) + 1 >= SoundLoader::Get().Wave(name.value())->size())
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
	if (read.size() < 0)
	{
		Stop();
		read.push_back(0);
		cnt = 1;
	}
}

// 代入演算子
void Okdio::operator=(const Okdio& okdio)
{
	info = okdio.info;

	if (FAILED(CreateVoice()))
	{
		return;
	}

	name   = okdio.name;
	index  = 0;
	cnt    = 1;
	loop   = false;
	handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

	read.assign(1, 0);
	wave.resize(BUFFER);

	UpData();
}

// 一回の処理データサイズ
inline constexpr unsigned int Okdio::Bps(void) const
{
	return info.sample * info.channel / OFFSET;
}

// データ読み込み前に呼び出し
void __stdcall Okdio::OnVoiceProcessingPassStart(unsigned int SamplesRequired)
{
	printf("%s\n", __func__);
	WaitForSingleObject(handle, INFINITE);
	Submit();
}

// 新しいバッファの処理開始時に呼び出し
void __stdcall Okdio::OnBufferStart(void* pBufferContext)
{
	printf("%s\n", __func__);
	UpData();
}

// バッファの処理終了時に呼び出し
void __stdcall Okdio::OnBufferEnd(void* pBufferContext)
{
	printf("%s\n", __func__);
	CheckEnd();
}

// 音声の処理パス終了時に呼び出し
void __stdcall Okdio::OnVoiceProcessingPassEnd()
{
	printf("%s\n", __func__);
	Reset();
}

// 連続したストリーム再生終了時に呼び出し
void __stdcall Okdio::OnStreamEnd()
{
}

// ループ終了位置到達時に呼び出し
void __stdcall Okdio::OnLoopEnd(void * pBufferContext)
{
}

// エラー発生時に呼び出し
void __stdcall Okdio::OnVoiceError(void * pBufferContext, long Error)
{
}
