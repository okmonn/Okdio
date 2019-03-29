#include "Pan.h"
#include "../../OKdio.h"

#define RAD(X) (X) * snd::PI() / 180.0f

// �R���X�g���N�^
Pan::Pan()
{
	Init();
}

// �R���X�g���N�^
Pan::Pan(const float& angle)
{
	Init();
	SetPanning(angle);
}

// �R�s�[�R���X�g���N�^
Pan::Pan(const Pan& pan)
{
	(*this) = pan;
}

// �f�X�g���N�^
Pan::~Pan()
{
}

// ������
void Pan::Init(void)
{
	angle = 0.0f;
}

// ��ʊp�x�Z�b�g
bool Pan::SetPanning(const float& angle)
{
	if (angle < -180.0f
		|| angle > 180.0f)
	{
		return false;
	}

	this->angle = angle;

	return true;
}

// ���s
void Pan::Execution(Okdio* okdio)
{
	if (okdio->GetInfo().channel <= 1 || angle == 0.0f)
	{
		return;
	}

	std::vector<float>& data = okdio->GetData();
	size_t ch = (angle > 0.0f) ? 0 : 1;
	for (size_t i = ch; i < data.size(); i += okdio->GetInfo().channel)
	{
		data[i] *= std::cos(RAD(angle * angle / 90.0f));
	}
}

// ������Z�q�I�[�o�[���[�h
void Pan::operator=(const Pan& pan)
{
	angle = pan.angle;
}
