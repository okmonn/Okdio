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
	InitFunc();

	if (func.find(type) != func.end())
	{
		func[type](cutoff, q, sample);
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

// �֐��|�C���^������
void Filter::InitFunc(void)
{
	func[snd::FilterType::LowPass]  = std::bind(&Filter::LowPass,  this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	func[snd::FilterType::HighPass] = std::bind(&Filter::HighPass, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	func[snd::FilterType::BandPass] = std::bind(&Filter::BandPass, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	func[snd::FilterType::Notch]    = std::bind(&Filter::Notch,    this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	func[snd::FilterType::LowShelf];
}

// ���[�p�X�t�B���^
bool Filter::LowPass(const float& cutoff, const float& q, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (q <= 0.0f)
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

// �n�C�p�X�t�B���^
bool Filter::HighPass(const float& cutoff, const float& q, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (q <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / 2.0f * q;

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  (1.0f + std::cos(omega)) / 2.0f;
	b[1] = -(1.0f + std::cos(omega));
	b[2] =  (1.0f + std::cos(omega)) / 2.0f;

	return true;
}

// �o���h�p�X�t�B���^
bool Filter::BandPass(const float& cutoff, const float& bw, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (bw <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) * std::sinh(std::log(2.0f) / 2.0f * bw * omega / std::sin(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  alpha;
	b[1] =  0.0f;
	b[2] = -alpha;

	return true;
}

// �m�b�`�t�B���^
bool Filter::Notch(const float& cutoff, const float& bw, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (bw <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) * std::sinh(std::log(2.0f) / 2.0f * bw * omega / std::sin(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  1.0f;
	b[1] = -2.0f * std::cos(omega);
	b[2] =  1.0f;

	return true;
}

// ���[�V�F���t�t�B���^
bool Filter::LowShelf(const float& cutoff, const float& q, const float& gain, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (q <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / 2.0f * q;
	float A     = std::pow(10.0f, gain / 40.0f);
	float beta  = std::sqrt(A) / q;

	a[0] = (A + 1.0f) + (A - 1.0f) * std::cos(omega) + beta * std::sin(omega);
	a[1] = -2.0f * ((A - 1.0f) + (A + 1.0f) * std::cos(omega));
	a[2] = (A + 1.0f) + (A - 1.0f) * std::cos(omega) - beta * std::sin(omega);

	b[0] = A * ((A + 1.0f) - (A - 1.0f) * std::cos(omega) + beta * std::sin(omega));
	b[1] = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * std::cos(omega));
	b[2] = A * ((A + 1.0f) - (A - 1.0f) * std::cos(omega) - beta * std::sin(omega));

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
