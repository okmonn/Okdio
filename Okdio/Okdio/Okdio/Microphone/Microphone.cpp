#include "Microphone.h"
#include "../Okdio.h"
#include "../Loader/Loader.h"

#pragma comment (lib, "winmm.lib")

Okdio* Microphone::okdio = nullptr;
bool Microphone::fin = false;
std::array<Microphone::WAVEIN, 2> Microphone::waveIn;
std::vector<float> Microphone::data;

// コンストラクタ
Microphone::Microphone() :
	InHandle(nullptr), info(44100, 16, 1)
{
	for (auto& i : waveIn)
	{
		i.Set(info);
	}
	data.resize(info.sample * info.channel);

	InOpen();
}

// デストラクタ
Microphone::~Microphone()
{
	InFinish();
}

// 入力コールバック
void __stdcall Microphone::WaveInProc(HWAVEIN handle, unsigned int msg, unsigned long instance, WPARAM param1, LPARAM param2)
{
	switch (msg)
	{
	case MM_WIM_DATA:
		if (fin == false)
		{
			short* tmp = (short*)((PWAVEHDR)param1)->lpData;
			data.assign(&tmp[0], &tmp[((PWAVEHDR)param1)->dwBufferLength / sizeof(tmp[0])]);
			for (float& i : data)
			{
				i /= float(0xffff / 2);
			}
			std::swap(*Loader::Get().Data(okdio->Name()).lock(), data);

			auto hr = waveInAddBuffer(handle, (PWAVEHDR)param1, sizeof(WAVEHDR));
			_ASSERT(hr == MMSYSERR_NOERROR);
		}
		break;
	default:
		break;
	}
}

// 入力デバイスオープン
void Microphone::InOpen(void)
{
	WAVEFORMATEX fmt{};
	fmt.wFormatTag      = WAVE_FORMAT_PCM;
	fmt.nChannels       = info.channel;
	fmt.nSamplesPerSec  = info.sample;
	fmt.wBitsPerSample  = info.bit;
	fmt.nBlockAlign     = fmt.wBitsPerSample * fmt.nChannels / 8;
	fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nBlockAlign;

	auto hr = waveInOpen(&InHandle, WAVE_MAPPER, &fmt, (DWORD_PTR)WaveInProc, 0, CALLBACK_FUNCTION);
	_ASSERT(hr == MMSYSERR_NOERROR);

	for (auto& i : waveIn)
	{
		hr = waveInPrepareHeader(InHandle, &i.hdr, sizeof(i.hdr));
		_ASSERT(hr == MMSYSERR_NOERROR);

		hr = waveInAddBuffer(InHandle, &i.hdr, sizeof(i.hdr));
		_ASSERT(hr == MMSYSERR_NOERROR);
	}
}

// 再生ターゲットセット
void Microphone::SetAudio(Okdio* okdio)
{
	this->okdio = okdio;
	this->okdio->CustomWave(info, data);
}

// 入力開始
void Microphone::Start(void)
{
	auto hr = waveInStart(InHandle);
	_ASSERT(hr == MMSYSERR_NOERROR);
}

// 入力終了
void Microphone::InFinish(void)
{
	fin = true;

	auto hr = waveInReset(InHandle);
	_ASSERT(hr == MMSYSERR_NOERROR);

	for (auto& i : waveIn)
	{
		hr = waveInUnprepareHeader(InHandle, &i.hdr, sizeof(i.hdr));
		_ASSERT(hr == MMSYSERR_NOERROR);
	}

	hr = waveInClose(InHandle);
	_ASSERT(hr == MMSYSERR_NOERROR);
}
