#pragma once
#include "../etc/Info.h"
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

// �����t�@�C���ǂݍ���
class Loader
{
public:
	// �f�X�g���N�^
	~Loader();

	// �t�@�C���ǂݍ���
	int Load(const std::string& fileName);

	// �I���W�i���g�`�o�^
	std::string Create(const snd::Info& info, const std::vector<float>& data);

	// �T�E���h�f�[�^�폜
	void Delete(const std::string& fileName);

	// �C���X�^���X�ϐ��擾
	static Loader& Get(void);

	// �T�E���h���擾
	snd::Info Info(const std::string& fileName);

	// �g�`�f�[�^�擾
	std::weak_ptr<std::vector<float>> Data(const std::string& fileName);

private:
	// �R���X�g���N�^
	Loader();
	Loader(const Loader&) = delete;
	void operator=(const Loader&) = delete;

	// ������
	void Init(void);


	// �I���W�i���g�`���ʖ�
	std::string original;

	// �I���W�i���o�^�i���o�[
	unsigned int cnt;

	// �T�E���h���
	std::unordered_map<std::string, snd::Info>info;

	// �g�`�f�[�^
	std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>data;

	// �֐��|�C���^
	std::unordered_map<std::string, std::function<int(const std::string&)>>func;
};
