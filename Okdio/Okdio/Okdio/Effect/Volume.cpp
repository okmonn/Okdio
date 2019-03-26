#include "Volume.h"

// �R���X�g���N�^
Volume::Volume()
{
	Init();
}

// �R���X�g���N�^
Volume::Volume(const float& level)
{
	Init();
	SetLevel(level);
}

// �R�s�[�R���X�g���N�^
Volume::Volume(const Volume& vol)
{
	(*this) = vol;
}

// �f�X�g���N�^
Volume::~Volume()
{
}

// ������
void Volume::Init(void)
{
	level = 1.0f;
}

// ���ʃ��x���Z�b�g
bool Volume::SetLevel(const float& level)
{
	if (level < 0.0f)
	{
		return false;
	}

	this->level = level;
	
	return false;
}

// ���s
void Volume::Execution(std::vector<float>& data)
{
	for (float& i : data)
	{
		i *= level;
	}
}

// ������Z�q�I�[�o�[���[�h
void Volume::operator=(const Volume& vol)
{
	level = vol.level;
}
