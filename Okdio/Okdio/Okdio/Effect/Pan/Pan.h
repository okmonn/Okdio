#pragma once
#include "../Effect.h"

// ��ʒ���
class Pan : 
	public Effect
{
public:
	// �R���X�g���N�^
	Pan();
	Pan(const float& angle);
	// �R�s�[�R���X�g���N�^
	Pan(const Pan& pan);
	// �f�X�g���N�^
	~Pan();

	// ��ʊp�x�Z�b�g
	//angle�@�ŏ��l�F-90.0f�A�ő�l�F90.0f
	bool SetPanning(const float& angle);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Pan& pan);

private:
	// ������
	void Init(void);

	// ���s
	void Execution(Okdio* okdio);


	// ��ʊp�x
	float angle;
};
