#include "TimeStretch.h"
#include "../../Okdio.h"

// �R���X�g���N�^
TimeStretch::TimeStretch()
{
	Init();
}

// �R���X�g���N�^
TimeStretch::TimeStretch(const float& speed)
{
	Init();
	SetSpeed(speed);
}

// �R�s�[�R���X�g���N�^
TimeStretch::TimeStretch(const TimeStretch& time)
{
	(*this) = time;
}

// �f�X�g���N�^
TimeStretch::~TimeStretch()
{
}

// �Đ����x�Z�b�g
bool TimeStretch::SetSpeed(const float& speed)
{
	if (speed <= 0.0f)
	{
		return false;
	}

	this->speed = speed;

	return true;
}

// ������
void TimeStretch::Init(void)
{
	speed = 1.0f;
}

// ���s
void TimeStretch::Execution(Okdio* okdio)
{
	//�u���b�N�T�C�Y
	const unsigned int block = 40;
	//���͈͍֔ŏ��l
	const unsigned int min = 1;
	//���͈͍֔ő�l
	const unsigned int max = 10;

	//���g�`�̃I�t�Z�b�g
	unsigned int offset1 = 0;
	//�K���g�`�̃I�t�Z�b�g
	unsigned int offset2 = 0;

	auto data = okdio->GetData();

	//�K���f�[�^
	std::vector<float>adap(data.size() / speed);

	while (offset1 < data.size())
	{
		float max_r = 0.0f;
		unsigned int offset = 0;
		for (unsigned int m = min; m <= max; ++m)
		{
			float tmp = 0.0f;
			for (unsigned int i = 0; i < block; ++i)
			{
				if (m + offset1 + i >= data.size())
				{
					break;
				}

				tmp += data[offset1 + i] * data[m + offset1 + i];
			}

			if (max_r < tmp)
			{
				tmp = max_r;
				offset = m;
			}
		}

		int q = offset / (speed - 1.0f) + 0.5f;
		for (unsigned int i = offset; i < q; ++i)
		{
			if (offset1 + i + offset > data.size())
			{
				break;
			}
			adap[offset2 + i] = data[offset1 + i + offset];
		}

		offset1 += offset + q;
		offset2 += offset;
	}

	okdio->GetData() = adap;
}

// ������Z�q�I�[�o�[���[�h
void TimeStretch::operator=(const TimeStretch& time)
{
	speed = time.speed;
}

