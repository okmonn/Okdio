#pragma once
#include <vector>

// �f�W�^���t�B���^
class Filter
{
public:
	// �R���X�g���N�^
	Filter();
	// �f�X�g���N�^
	~Filter();

	// ���[�p�X
	void LowPass(const unsigned int& sample, const float& cutoff, const float& q);

	// �n�C�p�X
	void HighPass(const unsigned int& sample, const float& cutoff, const float& q);

	// �o���h�p�X
	void BandPass(const unsigned int& sample, const float& cutoff, const float& bw);

	// �m�b�`�t�B���^
	void Notch(const unsigned int& sample, const float& cutoff, const float& bw);

	// ���[�V�F���t
	void LowShelf(const unsigned int& sample, const float& cutoff, const float& gain, const float& q);

	// �n�C�V�F���t
	void HighShelf(const unsigned int& sample, const float& cutoff, const float& gain, const float& q);

	// �s�[�L���O
	void Peaking(const unsigned int& sample, const float& cutoff, const float& gain, const float& bw);

	// �I�[���p�X
	void AllPass(const unsigned int& sample, const float& cutoff, const float& q);

	// ���s
	void Execution(std::vector<float>& data, float* input, float* output);

private:
	Filter(const Filter&) = delete;
	void operator=(const Filter&) = delete;


	// �t�B���^�W��A
	float a[3];

	// �t�B���^�W��B
	float b[3];
};
