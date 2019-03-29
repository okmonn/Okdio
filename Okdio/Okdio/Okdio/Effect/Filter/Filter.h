#pragma once
#include "../Effect.h"
#include <functional>
#include <unordered_map>

namespace snd
{
	// �t�B���^�^�C�v
	enum class FilterType
	{
		LowPass,
		HighPass,
		BandPass,
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
	// cutoff�@�ŏ��l�F10.0f�A�ő�l�Fsample�̔��������i10�̈ʂ���؂�̂Ă��s���Ă��܂��j
	// q�@0.0f���傫�����l�ɐݒ肵�Ă�������
	// sample�@�K��������g�`�̃T���v�����O���g��
	bool LowPass(const float& cutoff, const float& q, const unsigned short& sample = 44100);

	// �n�C�p�X�t�B���^
	// cutoff�@�ŏ��l�F10.0f�A�ő�l�Fsample�̔��������i10�̈ʂ���؂�̂Ă��s���Ă��܂��j
	// q�@0.0f���傫�����l�ɐݒ肵�Ă�������
	// sample�@�K��������g�`�̃T���v�����O���g��
	bool HighPass(const float& cutoff, const float& q, const unsigned short& sample = 44100);

	// �o���h�p�X�t�B���^
	// cutoff�@�ŏ��l�F10.0f�A�ő�l�Fsample�̔��������i10�̈ʂ���؂�̂Ă��s���Ă��܂��j
	// bw�@0.0f���傫�����l�ɐݒ肵�Ă�������
	// sample�@�K��������g�`�̃T���v�����O���g��
	bool BandPass(const float& cutoff, const float& bw, const unsigned short& sample = 44100);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Filter& filter);

private:
	// ������
	void Init(void);

	// �֐��|�C���^������
	void InitFunc(void);

	// ���s
	void Execution(Okdio* okdio);


	// �t�B���^�W��
	float a[3];

	// �t�B���^�W��
	float b[3];

	// �֐��|�C���^
	std::unordered_map<snd::FilterType, std::function<bool(const float&, const float&, const unsigned short&)>>func;
};
