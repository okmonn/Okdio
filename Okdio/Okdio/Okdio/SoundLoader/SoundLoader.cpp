#include "SoundLoader.h"
#include "../WAVE/WAVE.h"

// �R���X�g���N�^
SoundLoader::SoundLoader()
{
	Init();
}

// �f�X�g���N�^
SoundLoader::~SoundLoader()
{
}

// ������
void SoundLoader::Init(void)
{
	func[".wav"] = [&](const std::string& fileName, snd::Info& info)->int {
		return wav::Load(fileName, info, wave[fileName]);
	};
}

// �ǂݍ���
int SoundLoader::Load(const std::string& fileName, snd::Info& info)
{
	std::string format = fileName.substr(fileName.find_last_of("."), fileName.size());
	if (func.find(format) == func.end())
	{
		return -1;
	}

	return func[format](fileName, info);
}

// �C���X�^���X�ϐ��擾
SoundLoader& SoundLoader::Get(void)
{
	static SoundLoader instance;
	return instance;
}

// �T�E���h���擾
snd::Info SoundLoader::Info(const std::string& fileName)
{
	if (info.find(fileName) == info.end())
	{
		return snd::Info(UINT_MAX);
	}

	return info[fileName];
}

// �T�E���h�f�[�^�擾
std::shared_ptr<std::vector<float>> SoundLoader::Wave(const std::string& fileName)
{
	if (wave.find(fileName) == wave.end())
	{
		return nullptr;
	}

	return wave[fileName];
}
