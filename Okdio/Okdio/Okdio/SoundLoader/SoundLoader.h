#pragma once
#include "../SoundInfo.h"
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

namespace snd {
	struct Info;
}

// �T�E���h�t�@�C���ǂݍ���
class SoundLoader
{
public:
	// �f�X�g���N�^
	~SoundLoader();

	// �ǂݍ���
	int Load(const std::string& fileName, snd::Info& info);

	// �C���X�^���X�ϐ��擾
	static SoundLoader& Get(void);
	// �T�E���h���擾
	snd::Info Info(const std::string& fileName);
	// �T�E���h�f�[�^�擾
	std::shared_ptr<std::vector<float>> Wave(const std::string& fileName);

private:
	// �R���X�g���N�^
	SoundLoader();
	SoundLoader(const SoundLoader&) = delete;
	void operator=(const SoundLoader&) = delete;

	// ������
	void Init(void);


	// �T�E���h���
	std::unordered_map<std::string, snd::Info>info;

	// �g�`�f�[�^
	std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>wave;

	// �֐��|�C���^
	std::unordered_map<std::string, std::function<int(const std::string&, snd::Info&)>>func;
};
