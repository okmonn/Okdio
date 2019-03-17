#include "Effector.h"
#include "../Okdio.h"
#include "../Filter/Filter.h"
#include <algorithm>

// �R���X�g���N�^
Effector::Effector(const unsigned int& threadNum, const unsigned int& capacity) : 
	capacity(capacity), threadFlag(true)
{
	Init();
	SetFunc();

	th.resize(threadNum);
	for (std::thread& i : th)
	{
		i = std::thread(&Effector::Stream, this);
	}
}

// �f�X�g���N�^
Effector::~Effector()
{
	threadFlag = false;
	cv.notify_all();
	for (std::thread& i : th)
	{
		if (i.joinable() == true)
		{
			i.join();
		}
	}
}

// ������
void Effector::Init(void)
{
	filter = std::make_unique<Filter>();
}

// ���[�p�X�t�B���^���s
void Effector::ExecutionLowPass(Okdio* okdio)
{
	filter->LowPass(okdio->info.sample, okdio->filter.cutoff, okdio->filter.bw);
	filter->Execution(okdio->Wave(), okdio->inout[0], okdio->inout[1]);
}

// �{�����[�����s
void Effector::ExecutionVolume(Okdio* okdio)
{
	for (float& i : okdio->Wave())
	{
		i *= okdio->volume;
	}
}

// �֐��|�C���^�Z�b�g
void Effector::SetFunc(void)
{
	func[snd::Effect::LowPass] = std::bind(&Effector::ExecutionLowPass, this, std::placeholders::_1);
	func[snd::Effect::Volume]  = std::bind(&Effector::ExecutionVolume, this, std::placeholders::_1);
}

// �L���[�ɒǉ�
bool Effector::AddQueue(Okdio* okdio)
{
	std::unique_lock<std::mutex>lock(mtx);
	if (queue.size() >= capacity)
	{
		return false;
	}

	queue.push_back(okdio);
	cv.notify_all();

	return true;
}

// �񓯊�
void Effector::Stream(void)
{
	while (threadFlag)
	{
		std::unique_lock<std::mutex>lock(mtx);
		while (queue.empty())
		{
			cv.wait(lock);
		}

		for (snd::Effect& type : queue.front()->type)
		{
			func[type](queue.front());
		}

		SetEvent(queue.front()->handle);
		queue.pop_front();
	}
}
