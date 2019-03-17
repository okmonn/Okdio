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

	// ���s
	void Execution(std::vector<float>& data, float* input, float* output);
	void Execution(float& data, float* input, float* output);

private:
	Filter(const Filter&) = delete;
	void operator=(const Filter&) = delete;


	// �t�B���^�W��A
	float a[3];

	// �t�B���^�W��B
	float b[3];
};
