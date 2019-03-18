#pragma once
#include "Queue.h"
#include "../SoundInfo.h"
#include <mutex>
#include <vector>
#include <memory>
#include <thread>
#include <functional>
#include <unordered_map>
#include <condition_variable>

class Okdio;
class Filter;

// �T�E���h�G�t�F�N�^�[
class Effector
{
public:
	// �R���X�g���N�^
	Effector(const unsigned int& threadNum, const unsigned int& queueCapacity);
	// �f�X�g���N�^
	~Effector();

	// �L���[�ɒǉ�
	bool Add(Okdio* okdio);

private:
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// ������
	void Init(void);

	// ���[�p�X�t�B���^���s
	void LowPass(Okdio* okdio);

	// �{�����[�����s
	void Volume(Okdio* okdio);

	// �֐��|�C���^�Z�b�g
	void SetFunc(void);

	// ���񏈗�
	void Stream(void);


	// �t�B���^�[
	std::unique_ptr<Filter>filter;

	// �X���b�h�t���O
	bool threadFlag;

	// �L���[
	Queue<Okdio*>queue;

	// �r������
	std::mutex mtx;

	// �ҋ@
	std::condition_variable cv;

	// �X���b�h
	std::vector<std::thread>th;

	// �֐��|�C���^
	std::unordered_map<snd::Effect, std::function<void(Okdio*)>>func;
};
