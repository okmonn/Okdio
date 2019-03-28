#include "Distortion.h"
#include "../../OKdio.h"

// �R���X�g���N�^
Distortion::Distortion()
{
	Init();
}

// �R���X�g���N�^
Distortion::Distortion(const float& amp)
{
	Init();
	SetAmp(amp);
}

// �R�s�[�R���X�g���N�^
Distortion::Distortion(const Distortion& dis)
{
	(*this) = dis;
}

// �f�X�g���N�^
Distortion::~Distortion()
{
}

// ������
void Distortion::Init(void)
{
	amp = 1.0f;
}

// �������Z�b�g
bool Distortion::SetAmp(const float& amp)
{
	if (amp < 1.0f)
	{
		return false;
	}

	this->amp = amp;

	return true;
}

// ���s
void Distortion::Execution(Okdio* okdio)
{
	for (float& i : okdio->GetData())
	{
		i *= amp;
		i = std::fmin(i,  1.0f);
		i = std::fmax(i, -1.0f);
	}
}

// ������Z�q�I�[�o�[���[�h
void Distortion::operator=(const Distortion& dis)
{
	amp = dis.amp;
}
