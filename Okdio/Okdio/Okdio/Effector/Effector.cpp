#include "Effector.h"
#include "../Okdio.h"
#include "../Filter/Filter.h"

// �R���X�g���N�^
Effector::Effector(const unsigned int& threadNum, const unsigned int& queueCapacity) : 
	threadFlag(true), queue(queueCapacity)
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
		i.join();
	}
}

// �L���[�ɒǉ�
bool Effector::Add(Okdio* okdio)
{
	{
		std::unique_lock<std::mutex>lock(mtx);
		if (!queue.Push(okdio))
		{
			return false;
		}
	}

	cv.notify_all();

	return true;
}

// ������
void Effector::Init(void)
{
	filter = std::make_unique<Filter>();
}

// ���[�p�X�t�B���^���s
void Effector::LowPass(Okdio* okdio)
{
	filter->LowPass(okdio->info.sample, okdio->filter.cutoff, okdio->filter.bw);
	filter->Execution(okdio->Wave(), okdio->inout[0], okdio->inout[1]);
}

// �{�����[�����s
void Effector::Volume(Okdio* okdio)
{
	for (float& i : okdio->Wave())
	{
		i *= okdio->volume;
	}
}

// �֐��|�C���^�Z�b�g
void Effector::SetFunc(void)
{
	func[snd::Effect::LowPass] = std::bind(&Effector::LowPass, this, std::placeholders::_1);
	func[snd::Effect::Volume]  = std::bind(&Effector::Volume, this, std::placeholders::_1);
}

// ���񏈗�
void Effector::Stream(void)
{
	while (threadFlag)
	{
		Okdio* okdio = nullptr;
		{
			std::unique_lock<std::mutex>lock(mtx);
			while (queue.Empty())
			{
				if (threadFlag == false)
				{
					return;
				}
				cv.wait(lock);
			}

			queue.Pop(okdio);
		}

		for (snd::Effect& i : okdio->type)
		{
			func[i](okdio);
		}

		SetEvent(okdio->handle);
	}
}
