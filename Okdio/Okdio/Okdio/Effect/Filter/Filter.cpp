#include "Filter.h"
#include "../../OKdio.h"
#include "../../etc/Func.h"

// �J�b�g�I�t���g���ŏ��l
#define CUT_MIN 10.0f

// �R���X�g���N�^
Filter::Filter()
{
	Init();
}

// �R���X�g���N�^
Filter::Filter(const snd::FilterType& type, const float& cutoff, const float& q, const unsigned short& sample)
{
	Init();

	if (type == snd::FilterType::LowPass)
	{
		LowPass(cutoff, q, sample);
	}
}

// �R�s�[�R���X�g���N�^
Filter::Filter(const Filter& filter)
{
	(*this) = filter;
}

// �f�X�g���N�^
Filter::~Filter()
{
}

// ������
void Filter::Init(void)
{
	memset(&a[0], 0, sizeof(a));
	memset(&b[0], 0, sizeof(b));

	a[0] = b[0] = 1.0f;
}

// ���[�p�X�t�B���^
bool Filter::LowPass(const float& cutoff, const float& q, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / 2.0f * q;

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] = (1.0f - std::cos(omega)) / 2.0f;
	b[1] =  1.0f - std::cos(omega);
	b[2] = (1.0f - std::cos(omega)) / 2.0f;

	return true;
}

// ���s
void Filter::Execution(Okdio* okdio)
{
	float* inout = okdio->GetInOut();
	for (float& i : okdio->GetData())
	{
		float tmp = i;

		i = b[0] / a[0] * tmp
		  + b[1] / a[0] * inout[0]
		  + b[2] / a[0] * inout[1]
		  - a[1] / a[0] * inout[2]
		  - a[2] / a[0] * inout[3];

		//���͒l�̍X�V
		inout[1] = inout[0];
		inout[0] = tmp;

		//�o�͒l�̍X�V
		inout[3] = inout[2];
		inout[2] = i;
	}
}

// ������Z�q�I�[�o�[���[�h
void Filter::operator=(const Filter& filter)
{
	memcpy(a, &filter.a[0], sizeof(a));
	memcpy(b, &filter.b[0], sizeof(b));
}
