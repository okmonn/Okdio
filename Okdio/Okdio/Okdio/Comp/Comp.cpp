#include "Comp.h"

// �R���X�g���N�^
Comp::Comp()
{
}

// �f�X�g���N�^
Comp::~Comp()
{
}

// ���~�b�^�[
void Comp::Limiter(const float& threshold, std::vector<float>& data)
{
	for (float& i : data)
	{
		if (i > threshold)
		{
			i = threshold;
		}
		else if (i < -threshold)
		{
			i = -threshold;
		}
	}
}

// �R���v���b�T�[
void Comp::Compressor(const float& threshold, const float& ratio, std::vector<float>& data)
{
	//������
	float gain = 1.0f / (threshold + (1.0f - threshold) * ratio);

	for (float& i : data)
	{
		//���k
		if (i > threshold)
		{
			i = threshold + (i - threshold) * ratio;
		}
		else if (i < -threshold)
		{
			i = -threshold + (i + threshold) * ratio;
		}

		//����
		i *= gain;
	}
}
