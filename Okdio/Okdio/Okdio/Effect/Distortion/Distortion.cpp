#include "Distortion.h"

// コンストラクタ
Distortion::Distortion()
{
}

// デストラクタ
Distortion::~Distortion()
{
}

// サウンドエフェクト処理
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
