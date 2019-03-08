#include "Okdio.h"
#include "XAudio2/XAudio2.h"
#include "SoundLoader/SoundLoader.h"
#include <ks.h>
#include <ksmedia.h>
#include <algorithm>
#include <tchar.h>

// ���������
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }
// �o�b�t�@��
#define BUFFER 2
// �I�t�Z�b�g
#define OFFSET 100

// �X�s�[�J�[�ݒ�p�z��
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
Okdio::Okdio(const std::string& fileName)
{
	Init();
	Load(fileName);
}

// �f�X�g���N�^
Okdio::~Okdio()
{
	Destroy(voice);
}

// ������
void Okdio::Init(void)
{
	voice = nullptr;
	name = std::nullopt;

	index = 0;
	cnt = 0;
	loop = false;
	bps = 0;

	read.push_back(0);
	wave.resize(BUFFER);
}

// �\�[�X�{�C�X����
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
		OutputDebugString(_T("\n�{�C�X�����F���s\n"));
	}
#endif

	return hr;
}

// �ǂݍ���
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
	bps = info.sample * info.channel / OFFSET;

	return 0;
}

// �Đ�
long Okdio::Play(const bool& loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugString(_T("\n�Đ��F���s\n"));
		return hr;
#endif
	}

	this->loop = loop;

	++cnt;
	if (read.size() < cnt)
	{
		read.push_back(0);
	}

	return hr;
}

// ��~
long Okdio::Stop(void)
{
	auto hr = voice->Stop();
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugString(_T("\n��~�F���s\n"));
	}
#endif

	return hr;
}

// �o�b�t�@�ɒǉ�
long Okdio::Submit(void)
{
	wave[index].assign(bps, 0.0f);
	for (unsigned int& i : read)
	{
		//�c��T�C�Y�v�Z
		unsigned int size = (unsigned int(SoundLoader::Get().Wave(name.value())->size()) - i > bps)
			? bps
			: unsigned int(SoundLoader::Get().Wave(name.value())->size()) - i - 1;

		std::transform(&SoundLoader::Get().Wave(name.value())->at(i), &SoundLoader::Get().Wave(name.value())->at(i + size), wave[index].begin(), wave[index].begin(), std::plus<float>());
	}

	//�o�b�t�@�ɒǉ�
	XAUDIO2_BUFFER buf{};
	buf.AudioBytes = unsigned int(sizeof(float) * wave[index].size());
	buf.pAudioData = (unsigned char*)(wave[index].data());

	auto hr = voice->SubmitSourceBuffer(&buf);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugString(_T("\n�o�b�t�@�ɒǉ��F���s\n"));
#endif
		return hr;
	}

	index = (index + 1 >= BUFFER) ? 0 : ++index;
	
	for (unsigned int& i : read)
	{
		i += bps;
	}

	return hr;
}

// �I���m�F
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

	if (read.size() < 0)
	{
		Stop();
		read.push_back(0);
		cnt = 0;
	}
}

// �V�����o�b�t�@�̏����J�n���ɌĂяo��
void __stdcall Okdio::OnBufferStart(void* pBufferContext)
{
}

// �o�b�t�@�̏����I�����ɌĂяo��
void __stdcall Okdio::OnBufferEnd(void* pBufferContext)
{
	CheckEnd();
}

// �f�[�^�ǂݍ��ݑO�ɌĂяo��
void __stdcall Okdio::OnVoiceProcessingPassStart(unsigned int SamplesRequired)
{
	Submit();
}

// �����̏����p�X�I�����ɌĂяo��
void __stdcall Okdio::OnVoiceProcessingPassEnd()
{
}

// �A�������X�g���[���Đ��I�����ɌĂяo��
void __stdcall Okdio::OnStreamEnd()
{
	
}

// ���[�v�I���ʒu���B���ɌĂяo��
void __stdcall Okdio::OnLoopEnd(void * pBufferContext)
{
}

// �G���[�������ɌĂяo��
void __stdcall Okdio::OnVoiceError(void * pBufferContext, long Error)
{
}
