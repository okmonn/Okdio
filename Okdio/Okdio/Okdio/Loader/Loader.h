#pragma once
#include "../etc/Singleton.h"
#include "../etc/Info.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class Loader :
	public Singleton<Loader>
{
	friend Singleton<Loader>;

	struct Data {
		okmonn::Info info;
		std::shared_ptr<std::vector<unsigned char>>wave;
	};

public:
	// �ǂݍ���
	int Load(const std::string& fileName);

	// �f�[�^�폜
	void Delete(const std::string& fileName);
	
	// �T�E���h���擾
	okmonn::Info Info(const std::string& fileName);

	// �g�`�f�[�^
	std::weak_ptr<std::vector<unsigned char>>Wave(const std::string& fileName);

	// �����f�[�^�ʎ擾
	size_t ProcessSize(const std::string& fileName, const unsigned char& offset = 100);

private:
	// �R���X�g���N�^
	Loader();
	// �f�X�g���N�^
	~Loader();


	// �f�[�^
	std::unordered_map<std::string, Data>data;
};
