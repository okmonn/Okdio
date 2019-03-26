#include "Effector.h"
#include "../OKdio.h"

// �R���X�g���N�^
Effector::Effector()
{
}

// �R���X�g���N�^
Effector::Effector(const size_t& capacity, const size_t& threadNum) : 
	capacity(capacity), threadFlag(true)
{
	th.resize(threadFlag);
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

// �L���[�̖����ɒǉ�
bool Effector::Push(Okdio* okdio)
{
	std::unique_lock<std::mutex>lock(mtx);

	if (queue.size() >= capacity)
	{
		return false;
	}

	queue.emplace_back(okdio);
	cv.notify_all();

	return true;
}

// �L���[�̐擪���o��
bool Effector::Pop(Okdio** okdio)
{
	std::unique_lock<std::mutex>lock(mtx);
	while (queue.empty())
	{
		if (!threadFlag)
		{
			return false;
		}
		cv.wait(lock);
	}

	*okdio = queue.front();
	queue.pop_front();

	return true;
}

// �G�t�F�N�g�������s
void Effector::Execution(Okdio** okdio)
{
	std::mutex mtx;
	std::unique_lock<std::mutex>lock(mtx);

	for (float& i : (*okdio)->Data())
	{
		i *= 10.0f;
	}

	SetEvent((*okdio)->handle);
}

// �񓯊�
void Effector::Stream(void)
{
	while (threadFlag)
	{
		Okdio* okdio = nullptr;

		if (!Pop(&okdio))
		{
			break;
		}

		Execution(&okdio);
	}
}
