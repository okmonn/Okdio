#pragma once
#include <list>
#include <vector>
#include <thread>

class Okdio;

// �G�t�F�N�^�[
class Effector
{
public:
	// �f�X�g���N�^
	~Effector();

	// ���s�L���[�ɒǉ�
	void Execution(Okdio* data);

	// �C���X�^���X�ϐ��擾
	static Effector& Get(void);

private:
	// �R���X�g���N�^
	Effector();
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// �񓯊�
	void Stream(void);

	
	// �X���b�h�t���O
	bool threadFlag;

	// �X���b�h
	std::thread th;

	// �L���[
	std::list<Okdio*>queue;
};
