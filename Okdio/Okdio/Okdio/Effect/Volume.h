#pragma once
#include "Effect.h"

class Volume
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
	bool SetLevel(const float& level);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Volume& vol);

private:
	// ������
	void Init(void);

	// ���s
	void Execution(std::vector<float>& data);


	// ���ʃ��x��
	float level;
};
