#include "Comp.h"

// コンストラクタ
Comp::Comp()
{
}

// デストラクタ
Comp::~Comp()
{
}

// リミッター
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

// コンプレッサー
void Comp::Compressor(const float& threshold, const float& ratio, std::vector<float>& data)
{
	//増幅率
	float gain = 1.0f / (threshold + (1.0f - threshold) * ratio);

	for (float& i : data)
	{
		//圧縮
		if (i > threshold)
		{
			i = threshold + (i - threshold) * ratio;
		}
		else if (i < -threshold)
		{
			i = -threshold + (i + threshold) * ratio;
		}

		//増幅
		i *= gain;
	}
}
