#pragma once
#include "Effects.h"
#include <deque>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>

class Okdio;

// �G�t�F�N�g�����̓K��
class Effector
{
public:
	// �R���X�g���N�^
	Effector(const size_t& capacity, const size_t& threadNum);
	// �f�X�g���N�^
	~Effector();

	// �L���[�̖����ɒǉ�
	bool Push(Okdio* okdio);

private:
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// �X���b�h���Ă�邩�`�F�b�N
	bool CheckThreadNum(const size_t& threadNum);

	// �L���[�̐擪���o��
	bool Pop(Okdio** okdio);

	// �G�t�F�N�g�������s
	void Execution(Okdio** okdio);

	// �񓯊�
	void Stream(void);


	// �r������
	std::mutex mtx;

	// �ҋ@
	std::condition_variable cv;

	// �L���[�ő吔
	size_t capacity;

	// �X���b�h�t���O
	bool threadFlag;

	// �X���b�h
	std::vector<std::thread>th;

	// �L���[
	std::deque<Okdio*>queue;
};