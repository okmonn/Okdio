#include "Distortion.h"

// �R���X�g���N�^
Distortion::Distortion()
{
}

// �f�X�g���N�^
Distortion::~Distortion()
{
}

// �T�E���h�G�t�F�N�g����
void Distortion::DoProcess(const snd::DistortionParam& param, float* __restrict data, unsigned __int32 fream, unsigned __int32 channel)
{
	for (unsigned int i = 0; i < fream * channel; ++i)
	{
		data[i] *= param.gain;
		if (data[i] > 1.0f)
		{
			data[i] = 1.0f;
		}
		else if (data[i] < -1.0f)
		{
			data[i] = -1.0f;
		}

		data[i] *= param.level;
	}
}
