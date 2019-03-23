#include "Loader.h"
#include "../etc/WaveFmt.h"

// �R���X�g���N�^
Loader::Loader() : 
	original("original"), cnt(0)
{
	Init();
}

// �f�X�g���N�^
Loader::~Loader()
{
}

// ������
void Loader::Init(void)
{
	func[".wav"] = [&](const std::string& fileName)->int {
		if (info.find(fileName) != info.end())
		{
			return 0;
		}

		int hr = wav::Load(fileName, info[fileName], data[fileName]);
		if (hr != 0)
		{
			info.erase(info.find(fileName));
			data.erase(data.find(fileName));
		}

		return hr;
	};
}

// �t�@�C���ǂݍ���
int Loader::Load(const std::string& fileName)
{
	std::string format = fileName.substr(fileName.find_last_of("."), fileName.size());
	if (func.find(format) == func.end())
	{
		return -1;
	}

	return func[format](fileName);
}

// �I���W�i���g�`�o�^
std::string Loader::Create(const snd::Info& info, const std::vector<float>& data)
{
	original += cnt++;
	this->info[original] = info;
	this->data[original] = std::make_shared<std::vector<float>>(data);

	return original;
}

// �C���X�^���X�ϐ��擾
Loader& Loader::Get(void)
{
	static Loader instance;
	return instance;
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
