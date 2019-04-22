#include "TimeStretch.h"
#include "../../Okdio.h"

// �R���X�g���N�^
TimeStretch::TimeStretch()
{
	Init();
}

// �R���X�g���N�^
TimeStretch::TimeStretch(const float& speed)
{
	Init();
	SetSpeed(speed);
}

// �R�s�[�R���X�g���N�^
TimeStretch::TimeStretch(const TimeStretch& time)
{
	(*this) = time;
}

// �f�X�g���N�^
TimeStretch::~TimeStretch()
{
}

// �Đ����x�Z�b�g
bool TimeStretch::SetSpeed(const float& speed)
{
	if (speed <= 0.0f)
	{
		return false;
	}

	this->speed = speed;

	return true;
}

// ������
void TimeStretch::Init(void)
{
	speed = 1.0f;
}

// ���s
void TimeStretch::Execution(Okdio* okdio)
{
	auto data = okdio->GetData();

	int m = 0;
}

// ������Z�q�I�[�o�[���[�h
void TimeStretch::operator=(const TimeStretch& time)
{
	speed = time.speed;
}

