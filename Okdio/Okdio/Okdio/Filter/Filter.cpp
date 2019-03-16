#include "Filter.h"
#include "../SoundFunc/SoundFunc.h"

// コンストラクタ
Filter::Filter()
{
	memset(&a[0], 0, sizeof(a));
	memset(&b[0], 0, sizeof(b));
	a[0] = b[0] = 1.0f;
}

// デストラクタ
Filter::~Filter()
{
}

// ローパス
void Filter::LowPass(const unsigned int& sample, const float& cutoff, const float& q)
{
	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / (2.0f * q);

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] = (1.0f - std::cos(omega)) / 2.0f;
	b[1] =  1.0f - std::cos(omega);
	b[2] = (1.0f - std::cos(omega)) / 2.0f;
}

// 実行
void Filter::Execution(std::vector<float>& data, float* input, float* output)
{
	for (float& i : data)
	{
		//入力値保持
		float tmp = i;

		//適応
		i = b[0] / a[0] * tmp
		  + b[1] / a[0] * input[0]
		  + b[2] / a[0] * input[1]
		  - a[1] / a[0] * output[0]
		  - a[2] / a[0] * output[1];

		//過去入力値更新
		input[1] = input[0];
		input[0] = tmp;

		//過去出力値更新
		output[1] = output[0];
		output[0] = i;
	}
}
