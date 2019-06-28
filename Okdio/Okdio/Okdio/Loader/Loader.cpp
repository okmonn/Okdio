#include "Loader.h"
#include "../etc/WavFmt.h"
#include "../Func/Func.h"
#include <mfapi.h>
#include <mfidl.h>
#include <crtdbg.h>
#include <algorithm>
#include <mfreadwrite.h>

#pragma comment(lib, "mfuuid")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "mfreadwrite.lib")

// �R���X�g���N�^
Loader::Loader() : 
	custom("Custom"), cnt(0), attribute(nullptr)
{
	InitWMF();
	Init();
}

// �f�X�g���N�^
Loader::~Loader()
{
	attribute->Release();
	MFShutdown();
}

// WMF������
void Loader::InitWMF(void)
{
	auto hr = MFStartup(MF_VERSION);
	_ASSERT(hr == S_OK);

	hr = MFCreateAttributes(&attribute, 1);
	_ASSERT(hr == S_OK);

	hr = attribute->SetUINT32(MF_LOW_LATENCY, true);
	_ASSERT(hr == S_OK);
}

// ������
void Loader::Init(void)
{
	func[".wav"] = [&](const std::string& fileName)->int {
		if (info.find(fileName) != info.end())
		{
			return 0;
		}

		auto hr = wav::Load(fileName, info[fileName], data[fileName]);
		if (hr != 0)
		{
			info.erase(info.find(fileName));
			data.erase(data.find(fileName));
		}

		return hr;
	};

	func[".mp3"] = func[".ogg"] = [&](const std::string& fileName)->int {
		if (info.find(fileName) != info.end())
		{
			return 0;
		}

		auto hr = LoadWMF(fileName);
		if (hr != 0)
		{
			info.erase(info.find(fileName));
			data.erase(data.find(fileName));
		}

		return hr;
	};
}

// ���k�����f�[�^�̓ǂݍ���
long Loader::LoadWMF(const std::string& fileName)
{
	//���\�[�X���[�_�[����
	IMFSourceReader* reader = nullptr;
	auto hr = MFCreateSourceReaderFromURL(snd::ChangeCode(fileName).c_str(), attribute, &reader);
	_ASSERT(hr == S_OK);

	//���ׂẴI�[�f�B�I�X�g���[��������
	hr = reader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, false);
	_ASSERT(hr == S_OK);
	//�ŏ��̃I�[�f�B�I�X�g���[����I��
	hr = reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, true);
	_ASSERT(hr == S_OK);

	//���f�B�A�t�@�C�����擾
	IMFMediaType* mediaType = nullptr;
	hr = reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &mediaType);
	_ASSERT(hr == S_OK);

	//�I�[�f�B�I�t�@�C�����`�F�b�N
	GUID type{};
	hr = mediaType->GetGUID(MF_MT_MAJOR_TYPE, &type);
	_ASSERT(hr == S_OK);
	//�I�[�f�B�I�t�@�C�������k����Ă��邩�`�F�b�N
	hr = mediaType->GetGUID(MF_MT_SUBTYPE, &type);
	_ASSERT(hr == S_OK);

	//���k����Ă��Ȃ�
	if (type == MFAudioFormat_Float || type == MFAudioFormat_PCM)
	{
		return S_FALSE;
	}

	//���f�B�A�^�C�v����
	hr = MFCreateMediaType(&mediaType);
	_ASSERT(hr == S_OK);

	//���W���[�^�C�v���Z�b�g
	hr = mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	_ASSERT(hr == S_OK);
	//�T�u�^�C�v���Z�b�g
	hr = mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	_ASSERT(hr == S_OK);

	//�R�[�f�b�N����
	hr = reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, mediaType);
	_ASSERT(hr == S_OK);

	hr = reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &mediaType);
	_ASSERT(hr == S_OK);

	WAVEFORMATEX* fmt = nullptr;
	hr = MFCreateWaveFormatExFromMFMediaType(mediaType, &fmt, nullptr);
	_ASSERT(hr == S_OK);

	info[fileName] = { unsigned short(fmt->nSamplesPerSec), unsigned char(fmt->wBitsPerSample), unsigned char(fmt->nChannels) };
	data[fileName] = std::make_shared<std::vector<float>>();

	IMFSample* sample      = nullptr;
	IMFMediaBuffer* buffer = nullptr;
	switch (fmt->wBitsPerSample)
	{
	case 8:
	{
		while(true)
		{
			unsigned char* tmp     = nullptr;
			unsigned long dataSize = 0;
			unsigned long flag     = 0;
			hr = reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flag, nullptr, &sample);
			_ASSERT(hr == S_OK);

			if ((flag & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
				|| (flag & MF_SOURCE_READERF_ENDOFSTREAM))
			{
				break;
			}

			hr = sample->ConvertToContiguousBuffer(&buffer);
			_ASSERT(hr == S_OK);

			hr = buffer->Lock((BYTE**)&tmp, nullptr, &dataSize);
			_ASSERT(hr == S_OK);

			std::copy(&tmp[0], &tmp[dataSize / sizeof(tmp[0])], std::back_inserter(*data[fileName]));

			hr = buffer->Unlock();
			_ASSERT(hr == S_OK);
		}
		for (float& i : *data[fileName])
		{
			i = i / float(0xff / 2) - 1.0f;
		}
		break;
	}
	case 16:
	{
		while (true)
		{
			short* tmp             = nullptr;
			unsigned long dataSize = 0;
			unsigned long flag     = 0;
			hr = reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flag, nullptr, &sample);
			_ASSERT(hr == S_OK);

			if ((flag & MF_SOURCE_READERF_CURRENTMEDIATYPECHANGED)
				|| (flag & MF_SOURCE_READERF_ENDOFSTREAM))
			{
				break;
			}

			hr = sample->ConvertToContiguousBuffer(&buffer);
			_ASSERT(hr == S_OK);

			hr = buffer->Lock((BYTE**)&tmp, nullptr, &dataSize);
			_ASSERT(hr == S_OK);

			std::copy(&tmp[0], &tmp[dataSize / sizeof(tmp[0])], std::back_inserter(*data[fileName]));

			hr = buffer->Unlock();
			_ASSERT(hr == S_OK);
		}
		for (float& i : *data[fileName])
		{
			i /= float(0xffff / 2);
		}
		break;
	}
	default:
		break;
	}

	return hr;
}

// �ǂݍ���
int Loader::Load(const std::string& fileName)
{
	std::string format = fileName.substr(fileName.find_last_of("."), fileName.size());
	if (func.find(format) == func.end())
	{
		return -1;
	}

	return func[format](fileName);
}

// �J�X�^���g�`�o�^
std::string Loader::Custom(const snd::Info& info, const std::vector<float>& data)
{
	custom += cnt++;
	this->info[custom] = info;
	this->data[custom] = std::make_shared<std::vector<float>>(data);

	return custom;
}

// �o�^�g�`�̕ύX
bool Loader::Change(const std::string& fileName, const std::vector<float>& data)
{
	if (info.find(fileName) != info.end())
	{
		this->data[fileName]->assign(data.begin(), data.end());
		return true;
	}

	return false;
}

// �g�`�f�[�^�폜
void Loader::Delete(const std::string& fileName)
{
	if (info.find(fileName) != info.end())
	{
		info.erase(info.find(fileName));
		data.erase(data.find(fileName));
	}
}

// �t�@�C���p�X�̊m�F
bool Loader::IsFilePath(const std::string& fileName)
{
	if (data.find(fileName) != data.end())
	{
		return true;
	}

	return false;
}

// �T�E���h���擾
snd::Info Loader::Info(const std::string& fileName)
{
	if (info.find(fileName) == info.end())
	{
		return snd::Info();
	}

	return info[fileName];
}

// �g�`�f�[�^�擾
std::weak_ptr<std::vector<float>> Loader::Data(const std::string& fileName)
{
	if (data.find(fileName) == data.end())
	{
		return std::weak_ptr<std::vector<float>>();
	}

	return data[fileName];
}

// �g�`�f�[�^���擾
size_t Loader::DataSize(const std::string& fileName)
{
	if (data.find(fileName) == data.end())
	{
		return size_t();
	}

	return data[fileName]->size();
}

// 1��̏����f�[�^�ʎ擾
size_t Loader::Size(const std::string& fileName)
{
	if (data.find(fileName) == data.end())
	{
		return size_t();
	}

	return size_t(info[fileName].sample * info[fileName].channel);
}
