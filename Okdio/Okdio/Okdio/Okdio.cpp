#include "Okdio.h"
#include "XAudio2/XAudio2.h"
#include "SoundLoader/SoundLoader.h"
#include "SoundFunc/SoundFunc.h"
#include <ks.h>
#include <tchar.h>
#include <algorithm>
#include <ksmedia.h>

// ���������
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }
// �g�`�f�[�^�i�[�o�b�t�@��
#define BUFFER 2
// �f�[�^�T�C�Y�I�t�Z�b�g
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
Okdio::Okdio()
{
	Init();
}

// �R���X�g���N�^
Okdio::Okdio(std::weak_ptr<Effector> effe) : 
	effe(effe)
{
	Init();
}

// �R���X�g���N�^
Okdio::Okdio(const std::string & fileName)
{
	Init();
	Load(fileName);
}

// �R���X�g���N�^
Okdio::Okdio(const snd::Info& info, const std::vector<float>& data)
{
	Init();
	SetInfo(info, data);
}

// �R�s�[�R���X�g���N�^
Okdio::Okdio(const Okdio& okdio)
{
	(*this) = okdio;
}

// �f�X�g���N�^
Okdio::~Okdio()
{
	Destroy(voice);
	CloseHandle(handle);
}

// ������
void Okdio::Init(void)
{
	voice  = nullptr;
	name   = std::nullopt;
	index  = 0;
	cnt    = 0;
	loop   = false;
	handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

	read.push_back(0);
	wave.resize(BUFFER);

	EffeInit();
}

// �G�t�F�N�g�p�����[�^������
void Okdio::EffeInit(void)
{
	memset(&inout[0], 0, sizeof(inout));
	filter = { 10.0f, 1.0f / std::sqrt(2.0f) };
	volume = 1.0f;
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

	UpData();

	return 0;
}

// �I���W�i�����Z�b�g
int Okdio::SetInfo(const snd::Info& info, const std::vector<float>& data)
{
	this->info = info;

	if (FAILED(CreateVoice()))
	{
		return -1;
	}

	original = data;

	UpData();

	return 0;
}

// �f�W�^���t�B���^�p�����[�^�Z�b�g
bool Okdio::SetFilterParam(const float& cutoff, const float& bw)
{
	if (bw <= 0.0f
		|| cutoff < 10.0f || snd::Floor(float(info.sample / 2), 3) < cutoff)
	{
		return false;
	}

	filter = { cutoff, bw };

	return true;
}

// �{�����[���Z�b�g
bool Okdio::SetVolume(const float& volume)
{
	if (volume < 0.0f)
	{
		return false;
	}

	this->volume = volume;

	return true;
}

// �G�t�F�N�g�Z�b�g
void Okdio::SetEffect(const std::initializer_list<snd::Effect>& type)
{
	this->type = type;
}

// �G�t�F�N�g�ǉ�
void Okdio::AddEffect(const snd::Effect& type)
{
	this->type.push_back(type);
}

// �Đ�
long Okdio::Play(const bool& loop)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugString(_T("\n�Đ��F���s\n"));
#endif
		return hr;
	}

	this->loop = loop;

	++cnt;
	if (cnt > read.size())
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

// �g�`�f�[�^�X�V
void Okdio::UpData(void)
{
	wave[index].assign(Bps(), 0.0f);

	//�f�[�^�S�̃T�C�Y
	const unsigned int dataSize = (name != std::nullopt)
		? unsigned int(SoundLoader::Get().Wave(name.value())->size())
		: unsigned int(original.size());

	for (unsigned int& i : read)
	{
		//�c��T�C�Y�v�Z
		unsigned int size = (dataSize - i >= Bps()) ? Bps() : dataSize - i;
		if (i + size >= dataSize)
		{
			continue;
		}

		std::vector<float>origin = (name != std::nullopt)
			? std::vector<float>(&SoundLoader::Get().Wave(name.value())->at(i), &SoundLoader::Get().Wave(name.value())->at(i + size))
			: std::vector<float>(&original[i], &original[i + size]);

		std::transform(origin.begin(), origin.end(), wave[index].begin(), wave[index].begin(), std::plus<float>());
	}

	if (!effe.expired())
	{
		while (effe.lock()->Add(this) == false)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

// �o�b�t�@�ɒǉ�
long Okdio::Submit(void)
{
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
		i += Bps();
	}

	return hr;
}

// �I���m�F
void Okdio::CheckEnd(void)
{
	//�f�[�^�S�̃T�C�Y
	const unsigned int dataSize = (name != std::nullopt)
		? unsigned int(SoundLoader::Get().Wave(name.value())->size())
		: unsigned int(original.size());

	for (std::vector<unsigned int>::iterator itr = read.begin(); itr != read.end();)
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

// �g�`�ǂݍ��݂̃��Z�b�g
void Okdio::Reset(void)
{
	if (read.size() <= 0)
	{
		Stop();
		read.push_back(0);
	}
}

// ���
void Okdio::operator=(const Okdio& okdio)
{
	info = okdio.info;

	if (FAILED(CreateVoice()))
	{
		return;
	}

	name     = okdio.name;
	index    = 0;
	cnt      = 0;
	loop     = false;
	handle   = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
	original = okdio.original;

	read.assign(1, 0);
	wave.resize(BUFFER);

	EffeInit();

	UpData();
}

// ���̏����f�[�^�擾
inline constexpr unsigned int Okdio::Bps(void) const
{
	return info.sample * info.channel / OFFSET;
}

// ���݂̔g�`�f�[�^
std::vector<float>& Okdio::Wave(void)
{
	return wave[index];
}

// �f�[�^�ǂݍ��ݑO�ɌĂяo��
void __stdcall Okdio::OnVoiceProcessingPassStart(unsigned int SamplesRequired)
{
	if (!effe.expired())
	{
		WaitForSingleObject(handle, INFINITE);
	}

	Submit();
}

// �V�����o�b�t�@�̏����J�n���ɌĂяo��
void __stdcall Okdio::OnBufferStart(void * pBufferContext)
{
	UpData();
}

// �o�b�t�@�̏����I�����ɌĂяo��
void __stdcall Okdio::OnBufferEnd(void * pBufferContext)
{
	CheckEnd();
}

// �����̏����p�X�I�����ɌĂяo��
void __stdcall Okdio::OnVoiceProcessingPassEnd()
{
	Reset();
}
