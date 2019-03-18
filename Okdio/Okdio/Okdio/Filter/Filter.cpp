#include "Filter.h"
#include "../SoundFunc/SoundFunc.h"

// �R���X�g���N�^
Filter::Filter()
{
	memset(&a[0], 0, sizeof(a));
	memset(&b[0], 0, sizeof(b));
	a[0] = b[0] = 1.0f;
}

// �f�X�g���N�^
Filter::~Filter()
{
}

// ���[�p�X
void Filter::LowPass(const unsigned int& sample, const float& cutoff, const float& q)
{
	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / (2.0f * q);

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] = (1.0f - std::cos(omega)) / 2.0f;
	b[1] =  1.0f - std::cos(omega);
	b[2] = (1.0f - std::cos(omega)) / 2.0f;
}

// �n�C�p�X
void Filter::HighPass(const unsigned int& sample, const float& cutoff, const float& q)
{
	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / (2.0f * q);

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  (1.0f + std::cos(omega)) / 2.0f;
	b[1] = -(1.0f + std::cos(omega));
	b[2] =  (1.0f + std::cos(omega)) / 2.0f;
}

// �o���h�p�X
void Filter::BandPass(const unsigned int & sample, const float & cutoff, const float & bw)
{
	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) * std::sinh(logf(2.0f) / 2.0f * bw * omega / std::sin(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  alpha;
	b[1] =  0.0f;
	b[2] = -alpha;
}

// �m�b�`�t�B���^
void Filter::Notch(const unsigned int& sample, const float& cutoff, const float& bw)
{
	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) * std::sinh(std::log(2.0f) / 2.0f * bw * omega / std::sin(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  1.0f;
	b[1] = -2.0f * std::cos(omega);
	b[2] =  1.0f;
}

// ���[�V�F���t
void Filter::LowShelf(const unsigned int& sample, const float& cutoff, const float& gain, const float& q)
{
	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / (2.0f * q);
	float A     = std::pow(10.0f, (gain / 40.0f));
	float beta  = std::sqrt(A) / q;

	a[0] =  (A + 1.0f) + (A - 1.0f) * std::cos(omega) + beta * std::sin(omega);
	a[1] = -2.0f                    * ((A - 1.0f) + (A + 1.0f) * std::cos(omega));
	a[2] =  (A + 1.0f) + (A - 1.0f) * std::cos(omega) - beta * std::sin(omega);

	b[0] = A * ((A + 1.0f) - (A - 1.0f) * std::cos(omega) + beta   * std::sin(omega));
	b[1] = 2.0f * A                     * ((A - 1.0f) - (A + 1.0f) * std::cos(omega));
	b[2] = A * ((A + 1.0f) - (A - 1.0f) * std::cos(omega) - beta   * std::sin(omega));
}

// �n�C�V�F���t
void Filter::HighShelf(const unsigned int& sample, const float& cutoff, const float& gain, const float& q)
{
	float omega = 2.0f * snd::PI() *  cutoff / sample;
	float alpha = std::sin(omega) / (2.0f * q);
	float A     = std::pow(10.0f, (gain / 40.0f));
	float beta  = std::sqrt(A) / q;

	a[0] = (A + 1.0f) - (A - 1.0f) * std::cos(omega) + beta   * std::sin(omega);
	a[1] =  2.0f                   * ((A - 1.0f) - (A + 1.0f) * std::cos(omega));
	a[2] = (A + 1.0f) - (A - 1.0f) * std::cos(omega) - beta   * std::sin(omega);

	b[0] =  A * ((A + 1.0f) + (A - 1.0f) * std::cos(omega) + beta   * std::sin(omega));
	b[1] = -2.0f * A                     * ((A - 1.0f) + (A + 1.0f) * std::cos(omega));
	b[2] =  A * ((A + 1.0f) + (A - 1.0f) * std::cos(omega) - beta   * std::sin(omega));
}

// �s�[�L���O
void Filter::Peaking(const unsigned int& sample, const float& cutoff, const float& gain, const float& bw)
{
	float omega = 2.0f * snd::PI() *  cutoff / sample;
	float alpha = std::sin(omega) * std::sinh(std::log(2.0f) / 2.0f * bw * omega / std::sin(omega));
	float A     = std::pow(10.0f, (gain / 40.0f));

	a[0] =  1.0f + alpha / A;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha / A;

	b[0] =  1.0f + alpha * A;
	b[1] = -2.0f * std::cos(omega);
	b[2] =  1.0f - alpha * A;
}

// �I�[���p�X
void Filter::AllPass(const unsigned int& sample, const float& cutoff, const float& q)
{
	float omega = 2.0f * snd::PI() *  cutoff / sample;
	float alpha = std::sin(omega) / (2.0f * q);

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  1.0f - alpha;
	b[1] = -2.0f * std::cos(omega);
	b[2] =  1.0f + alpha;
}

// ���s
void Filter::Execution(std::vector<float>& data, float* input, float* output)
{
	for (float& i : data)
	{
		//���͒l�ێ�
		float tmp = i;

		//�K��
		i = b[0] / a[0] * tmp
		  + b[1] / a[0] * input[0]
		  + b[2] / a[0] * input[1]
		  - a[1] / a[0] * output[0]
		  - a[2] / a[0] * output[1];

		//�ߋ����͒l�X�V
		input[1] = input[0];
		input[0] = tmp;

		//�ߋ��o�͒l�X�V
		output[1] = output[0];
		output[0] = i;
	}
}
