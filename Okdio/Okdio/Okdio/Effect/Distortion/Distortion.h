#pragma once
#include "../Effect.h"

// �f�B�X�g�[�V��������
class Distortion :
	public Effect
{
public:
	// �R���X�g���N�^
	Distortion();
	Distortion(const float& amp);
	// �R�s�[�R���X�g���N�^
	Distortion(const Distortion& dis);
	// �f�X�g���N�^
	~Distortion();

	// �������Z�b�g
	// amp�@�ŏ��l�F1.0f
	bool SetAmp(const float& amp);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Distortion& dis);

private:
	// ������
	void Init(void);

	// ���s
	void Execution(Okdio* okdio);


	// ������
	float amp;
};
