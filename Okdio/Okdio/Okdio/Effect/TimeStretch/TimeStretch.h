#pragma once
#include "../Effect.h"

class TimeStretch :
	public Effect
{
public:
	// �R���X�g���N�^
	TimeStretch();
	TimeStretch(const float& speed);
	// �R�s�[�R���X�g���N�^
	TimeStretch(const TimeStretch& time);
	// �f�X�g���N�^
	~TimeStretch();

	// �Đ����x�Z�b�g
	// speed�@0.0f���傫���l
	bool SetSpeed(const float& speed);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const TimeStretch& time);

private:
	// ������
	void Init(void);

	// ���s
	void Execution(Okdio* okdio);


	// �Đ����x
	float speed;
};
