#include "Okdio.h"
#include "XAudio2/XAudio2.h"
#include "Loader/Loader.h"
#include "XAudio2/VoiceCallback.h"
#include <ks.h>
#include <ksmedia.h>

#pragma comment(lib, "xaudio2.lib")

// �X�s�[�J�ꗗ
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

// �R���X�g���N�^
Okdio::Okdio() : 
	read(0), loop(false), end(true)
{
	callback = std::make_unique<VoiceCallback>(this);
}

// �f�X�g���N�^
Okdio::~Okdio()
{
	if (voice != nullptr)
	{
		Stop();
		voice->DestroyVoice();
		voice = nullptr;
	}
}

// �\�[�X�{�C�X����
long Okdio::CreateVoice(void)
{
	okmonn::Info info = Loader::Get().Info(name);

	WAVEFORMATEXTENSIBLE fmt{};
	fmt.Format.cbSize          = sizeof(WAVEFORMATEXTENSIBLE) - sizeof(WAVEFORMATEX);
	fmt.Format.nSamplesPerSec  = info.sample;
	fmt.Format.wBitsPerSample  = info.bit;
	fmt.Format.nChannels       = info.channel;
	fmt.Format.nBlockAlign     = fmt.Format.nChannels * fmt.Format.wBitsPerSample / 8;
	fmt.Format.nAvgBytesPerSec = fmt.Format.nSamplesPerSec * fmt.Format.nBlockAlign;
	fmt.Format.wFormatTag      = WAVE_FORMAT_EXTENSIBLE;

	fmt.dwChannelMask               = spk[fmt.Format.nChannels - 1];
	fmt.Samples.wValidBitsPerSample = fmt.Format.wBitsPerSample;
	fmt.SubFormat                   = KSDATAFORMAT_SUBTYPE_PCM;

	return XAudio2::Get().Audio()->CreateSourceVoice(&voice, (WAVEFORMATEX*)& fmt, XAUDIO2_VOICE_USEFILTER, XAUDIO2_DEFAULT_FREQ_RATIO, &(*callback));
}

// �ǂݍ���
long Okdio::Load(const std::string& fileName)
{
	auto hr = Loader::Get().Load(fileName);
	if (FAILED(hr))
	{
		return hr;
	}

	name = fileName;
	hr = CreateVoice();
	if (FAILED(hr))
	{
		okmonn::DebugStream("�\�[�X�{�C�X�����F���s");
	}

	return hr;
}

// �Đ�
long Okdio::Play(const bool& loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
		okmonn::DebugStream("�Đ��J�n�F���s");
		return hr;
	}

	this->loop = loop; 
	end        = false;

	return hr;
}

// ��~
long Okdio::Stop(void)
{
	auto hr = voice->Stop(XAUDIO2_PLAY_TAILS);
	if (FAILED(hr))
	{
		okmonn::DebugStream("�Đ���~�F���s");
	}

	return hr;
}

// �I�[�f�B�I�o�b�t�@�ɒǉ�
void Okdio::Submit(void)
{
	XAUDIO2_VOICE_STATE st{};
	voice->GetState(&st);
	if (st.BuffersQueued != 0)
	{
		return;
	}

	auto wave = Loader::Get().Wave(name);
	size_t size = read + Loader::Get().ProcessSize(name) >= wave.lock()->size()
		? (read + Loader::Get().ProcessSize(name)) - wave.lock()->size() : Loader::Get().ProcessSize(name);

	XAUDIO2_BUFFER buf{};
	buf.AudioBytes = sizeof(wave.lock()->at(0)) * size;
	buf.pAudioData = (unsigned char*)&wave.lock()->at(read);
	auto hr = voice->SubmitSourceBuffer(&buf);
	if (FAILED(hr))
	{
		okmonn::DebugStream("�I�[�f�B�I�o�b�t�@�ɒǉ��F���s");
		return;
	}

	read += size;
}

// �I���`�F�b�N
void Okdio::CheckEnd(void)
{
	if (read >= Loader::Get().Wave(name).lock()->size())
	{
		if (loop != true)
		{
			Stop();
			end = true;
		}

		read = 0;
	}
}
