#pragma once
#include "../Effect.h"

// ���ʒ���
class Volume : 
	public Effect
{
public:
	// �R���X�g���N�^
	Volume();
	Volume(const float& level);
	// �R�s�[�R���X�g���N�^
	Volume(const Volume& vol);
	// �f�X�g���N�^
	~Volume();

	// ���ʃ��x���Z�b�g
	// level�@�ŏ��l�F0.0f�A�ő�l�F1.0f
	bool SetLevel(const float& level);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Volume& vol);

private:
	// ������
	void Init(void);

	// ���s
	void Execution(Okdio* okdio);


	// ���ʃ��x��
	float level;
};
