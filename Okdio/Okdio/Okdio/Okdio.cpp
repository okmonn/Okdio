#include "OKdio.h"
#include "Effector.h"
#include "XAudio2/XAudio2.h"
#include "XAudio2/VoiceCallback.h"
#include "Loader/Loader.h"
#include <ks.h>
#include <ksmedia.h>
#include <algorithm>

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

// �o�b�t�@�ő吔
#define BUFFER 2

// �R���X�g���N�^
Okdio::Okdio()
{
	Init();
}

// �R���X�g���N�^
Okdio::Okdio(Effector* effector)
{
	Init();

	this->effector = effector;
}

// �R���X�g���N�^
Okdio::Okdio(const std::string& fileName)
{
	Init();
	Load(fileName);
}

// �R���X�g���N�^
Okdio::Okdio(const snd::Info& info, const std::vector<float>& data)
{
	Init();
	CreateOriginal(info, data);
}

// �R���X�g���N�^
Okdio::Okdio(const std::string& fileName, Effector* effector)
{
	Init();
	this->effector = effector;
	Load(fileName);
}

// �R���X�g���N�^
Okdio::Okdio(const snd::Info& info, const std::vector<float>& data, Effector* effector)
{
	Init();
	this->effector = effector;
	CreateOriginal(info, data);
}

// �R�s�[�R���X�g���N�^
Okdio::Okdio(const Okdio& okdio)
{
	(*this) = okdio;
}

// �f�X�g���N�^
Okdio::~Okdio()
{
	if (voice != nullptr)
	{
		voice->DestroyVoice();
	}
}

// ������
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

// �\�[�X�{�C�X����
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
		OutputDebugStringA("\n�\�[�X�{�C�X�����F���s\n");
	}
#endif

	return hr;
}

// �t�@�C������ǂݍ���
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

// �I���W�i���g�`����
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

// �G�t�F�N�g�𖖔��ɒǉ�
void Okdio::PushEffect(Effect* effect)
{
	this->effect.emplace_back(effect);
}

// �G�t�F�N�g���܂Ƃ߂ăZ�b�g
void Okdio::SetEffect(const std::initializer_list<Effect*>& effect)
{
	this->effect = effect;

	UpData();
}

// �Đ�
long Okdio::Play(const bool& loop, const size_t& overlaidMax)
{
	auto hr = voice->Start();
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugStringA("\n�Đ��F���s\n");
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

// ��~
long Okdio::Stop(void)
{
	auto hr = voice->Stop();
	if (FAILED(hr))
	{
		OutputDebugStringA("\n��~�F���s\n");
	}

	return hr;
}

// �g�`�f�[�^���{�C�X�o�b�t�@�ɒǉ�
long Okdio::Submit(void)
{
	XAUDIO2_BUFFER buf{};
	buf.AudioBytes = unsigned int(sizeof(float) * data[index].size());
	buf.pAudioData = (unsigned char*)(data[index].data());

	auto hr = voice->SubmitSourceBuffer(&buf);
	if (FAILED(hr))
	{
#ifdef _DEBUG
		OutputDebugStringA("\n�\�[�X�{�C�X�̃o�b�t�@�ǉ��F���s\n");
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

// �g�`�f�[�^�X�V
void Okdio::UpData(void)
{
	data[index].assign(Bps(), 0.0f);

	//�f�[�^�S�̃T�C�Y
	const size_t dataSize = Loader::Get().Data(name).lock()->size();

	for (size_t& i : read)
	{
		//�c��T�C�Y
		const size_t size = (dataSize - i >= Bps()) ? Bps() : dataSize - i;
		if (i + size >= dataSize)
		{
			continue;
		}

		std::transform(&Loader::Get().Data(name).lock()->at(i), &Loader::Get().Data(name).lock()->at(i + size), data[index].begin(), data[index].begin(), std::plus<float>());
	}

	//�G�t�F�N�^�[�ɒǉ�
	if (effector != nullptr)
	{
		effector->Push(this);
	}
}

// �I���m�F
void Okdio::CheckEnd(void)
{
	//�f�[�^�S�̃T�C�Y
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

// ���Z�b�g
void Okdio::Reset(void)
{
	if (read.size() <= 0)
	{
		Stop();
		read.push_back(0);
	}
}

// ���̏����f�[�^�擾
inline size_t Okdio::Bps(void) const
{
	snd::Info info = Loader::Get().Info(name);
	return info.sample * info.channel / 100;
}

// ���݂̔g�`���擾
std::vector<float>& Okdio::Data(void)
{
	return data[index];
}

// ������Z�q�I�[�o�[���[�h
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
