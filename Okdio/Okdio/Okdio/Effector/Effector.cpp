#include "Effector.h"
#include "../Okdio.h"
#include "../Filter/Filter.h"
#include "../Comp/Comp.h"

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
	comp   = std::make_unique<Comp>();
}

// ���[�p�X�t�B���^���s
void Effector::LowPass(Okdio* okdio)
{
	filter->LowPass(okdio->info.sample, okdio->filter.cutoff, okdio->filter.bw);
	filter->Execution(okdio->Wave(), okdio->inout[0], okdio->inout[1]);
}

// �n�C�p�X�t�B���^���s
void Effector::HighPass(Okdio* okdio)
{
	filter->HighPass(okdio->info.sample, okdio->filter.cutoff, okdio->filter.bw);
	filter->Execution(okdio->Wave(), okdio->inout[0], okdio->inout[1]);
}

// �o���h�p�X���s
void Effector::BandPass(Okdio* okdio)
{
	filter->BandPass(okdio->info.sample, okdio->filter.cutoff, okdio->filter.bw);
	filter->Execution(okdio->Wave(), okdio->inout[0], okdio->inout[1]);
}

// ���~�b�^�[���s
void Effector::Limiter(Okdio* okdio)
{
	comp->Limiter(okdio->comp.threshold, okdio->Wave());
}

// �R���v���b�T�[���s
void Effector::Compressor(Okdio* okdio)
{
	comp->Compressor(okdio->comp.threshold, okdio->comp.ratio, okdio->Wave());
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
	func[snd::Effect::LowPass]  = std::bind(&Effector::LowPass,  this, std::placeholders::_1);
	func[snd::Effect::HighPass] = std::bind(&Effector::HighPass, this, std::placeholders::_1);
	func[snd::Effect::BandPass] = std::bind(&Effector::BandPass, this, std::placeholders::_1);

	func[snd::Effect::Limiter]  = std::bind(&Effector::Limiter, this, std::placeholders::_1);
	func[snd::Effect::Comp]     = std::bind(&Effector::Compressor, this, std::placeholders::_1);

	func[snd::Effect::Volume]   = std::bind(&Effector::Volume,   this, std::placeholders::_1);
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
			if (func.find(i) == func.end())
			{
				continue;
			}
			func[i](okdio);
		}

		SetEvent(okdio->handle);
	}
}
