#pragma once
#include "../Effect.h"

namespace snd
{
	// �t�B���^�^�C�v
	enum FilterType
	{
		LowPass,
	};
}

// �f�W�^���t�B���^
class Filter : 
	public Effect
{
public:
	// �R���X�g���N�^
	Filter();
	Filter(const snd::FilterType& type, const float& cutoff, const float& q, const unsigned short& sample = 44100);
	// �R�s�[�R���X�g���N�^
	Filter(const Filter& filter);
	// �f�X�g���N�^
	~Filter();

	// ���[�p�X�t�B���^
	bool LowPass(const float& cutoff, const float& q, const unsigned short& sample = 44100);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Filter& filter);

private:
	// ������
	void Init(void);

	// ���s
	void Execution(Okdio* okdio);


	// �t�B���^�W��
	float a[3];

	// �t�B���^�W��
	float b[3];
};
