#pragma once
#include "../SoundInfo.h"
#include <deque>
#include <mutex>
#include <vector>
#include <thread>
#include <functional>
#include <unordered_map>
#include <condition_variable>

class Okdio;
class Filter;

// �T�E���h�G�t�F�N�g���S�N���X
class Effector
{
public:
	// �R���X�g���N�^
	Effector(const unsigned int& threadNum, const unsigned int& capacity);
	// �f�X�g���N�^
	~Effector();

	// �L���[�ɒǉ�
	bool AddQueue(Okdio* okdio);

private:
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// ������
	void Init(void);

	// ���[�p�X�t�B���^���s
	void ExecutionLowPass(Okdio* okdio);

	// �{�����[�����s
	void ExecutionVolume(Okdio* okdio);

	// �֐��|�C���^�Z�b�g
	void SetFunc(void);

	// �񓯊�
	void Stream(void);


	// �f�W�^���t�B���^
	std::unique_ptr<Filter>filter;

	// �r������
	std::mutex mtx;

	// �ҋ@
	std::condition_variable cv;

	// �L���[�ő吔
	unsigned int capacity;

	// �X���b�h�t���O
	bool threadFlag;

	// �L���[
	std::deque<Okdio*>queue;

	// �X���b�h
	std::vector<std::thread>th;

	// �֐��|�C���^
	std::unordered_map<snd::Effect, std::function<void(Okdio*)>>func;
};
