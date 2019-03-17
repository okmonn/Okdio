#pragma once
#include <vector>

// デジタルフィルタ
class Filter
{
public:
	// コンストラクタ
	Filter();
	// デストラクタ
	~Filter();

	// ローパス
	void LowPass(const unsigned int& sample, const float& cutoff, const float& q);

	// 実行
	void Execution(std::vector<float>& data, float* input, float* output);
	void Execution(float& data, float* input, float* output);

private:
	Filter(const Filter&) = delete;
	void operator=(const Filter&) = delete;


	// フィルタ係数A
	float a[3];

	// フィルタ係数B
	float b[3];
};
