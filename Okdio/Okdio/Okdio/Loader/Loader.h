#pragma once
#include "../etc/Singleton.h"
#include "../etc/Info.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

struct IMFAttributes;

class Loader : 
	public Singleton<Loader>
{
	friend Singleton<Loader>;
public:
	// �ǂݍ���
	int Load(const std::string& fileName);

	// �J�X�^���g�`�o�^
	std::string Custom(const snd::Info& info, const std::vector<float>& data);

	// �o�^�g�`�̕ύX
	bool Change(const std::string& fileName, const std::vector<float>& data);

	// �g�`�f�[�^�폜
	void Delete(const std::string& fileName);

	// �t�@�C���p�X�̊m�F
	bool IsFilePath(const std::string& fileName);

	// �T�E���h���擾
	snd::Info Info(const std::string& fileName);

	// �g�`�f�[�^�擾
	std::weak_ptr<std::vector<float>> Data(const std::string& fileName);

	// �g�`�f�[�^���擾
	size_t DataSize(const std::string& fileName);

	// 1��̏����f�[�^�ʎ擾
	size_t Size(const std::string& fileName);

private:
	// �R���X�g���N�^
	Loader();
	// �f�X�g���N�^
	~Loader();

	// WMF������
	void InitWMF(void);

	// ������
	void Init(void);

	// ���k�����f�[�^�̓ǂݍ���
	long LoadWMF(const std::string& fileName);


	// �J�X�^���g�`���ʖ�
	std::string custom;

	// �J�X�^���g�`�o�^��
	unsigned char cnt;

	// �����l
	IMFAttributes* attribute;

	// �T�E���h���
	std::unordered_map<std::string, snd::Info>info;

	// �g�`�f�[�^
	std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>data;

	// �֐��|�C���^
	std::unordered_map<std::string, std::function<int(const std::string&)>>func;
};
