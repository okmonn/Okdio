#include "Effector.h"
#include "../Okdio.h"

// �R���X�g���N�^
Effector::Effector() : 
	threadFlag(true)
{
	th = std::thread(&Effector::Stream, this);
}

// �f�X�g���N�^
Effector::~Effector()
{
	if (th.joinable() == true)
	{
		threadFlag = false;
		th.join();
	}
}

// ���s�L���[�ɒǉ�
void Effector::Execution(Okdio* okdio)
{
	queue.push_back(okdio);
}

// �񓯊�
void Effector::Stream(void)
{
	while (threadFlag)
	{
		if (queue.empty())
		{
			continue;
		}

		for (float& i : (*queue.begin())->wave)
		{
			i = 0.0f;
		}

		SetEvent((*queue.begin())->handle);
		queue.erase(queue.begin());
	}
}

// �C���X�^���X�ϐ��擾
Effector & Effector::Get(void)
{
	static Effector instance;
	return instance; 
}
