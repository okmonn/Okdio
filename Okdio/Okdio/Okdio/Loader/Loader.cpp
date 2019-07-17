#include "Loader.h"
#include "../etc/WavFmt.h"
#include "../Func/Func.h"

// �R���X�g���N�^
Loader::Loader()
{
}

// �f�X�g���N�^
Loader::~Loader()
{
}

// �ǂݍ���
int Loader::Load(const std::string& fileName)
{
	auto hr = wav::Load(fileName, data[fileName].info, data[fileName].wave);
	if (hr != 0)
	{
		Delete(fileName);
		okmonn::DebugStream(fileName + "�̓ǂݍ��݁F���s");
		return -1;
	}

	return 0;
}

// �f�[�^�폜
void Loader::Delete(const std::string& fileName)
{
	if (data.find(fileName) != data.end())
	{
		data.erase(data.find(fileName));
	}
}

// �T�E���h���擾
okmonn::Info Loader::Info(const std::string& fileName)
{
	if (data.find(fileName) != data.end())
	{
		return data[fileName].info;
	}

	return okmonn::Info();
}

// �g�`�f�[�^
std::weak_ptr<std::vector<unsigned char>> Loader::Wave(const std::string& fileName)
{
	if (data.find(fileName) != data.end())
	{
		return data[fileName].wave;
	}

	return std::weak_ptr<std::vector<unsigned char>>();
}

// �����f�[�^�ʎ擾
size_t Loader::ProcessSize(const std::string& fileName, const unsigned char& offset)
{
	if (data.find(fileName) != data.end())
	{
		okmonn::Info& info = data[fileName].info;
		return size_t((info.sample * info.channel * (info.bit / 8)) / offset);
	}

	return size_t();
}
