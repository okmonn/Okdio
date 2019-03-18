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

	// ハイパス
	void HighPass(const unsigned int& sample, const float& cutoff, const float& q);

	// バンドパス
	void BandPass(const unsigned int& sample, const float& cutoff, const float& bw);

	// ノッチフィルタ
	void Notch(const unsigned int& sample, const float& cutoff, const float& bw);

	// ローシェルフ
	void LowShelf(const unsigned int& sample, const float& cutoff, const float& gain, const float& q);

	// ハイシェルフ
	void HighShelf(const unsigned int& sample, const float& cutoff, const float& gain, const float& q);

	// ピーキング
	void Peaking(const unsigned int& sample, const float& cutoff, const float& gain, const float& bw);

	// オールパス
	void AllPass(const unsigned int& sample, const float& cutoff, const float& q);

	// 実行
	void Execution(std::vector<float>& data, float* input, float* output);

private:
	Filter(const Filter&) = delete;
	void operator=(const Filter&) = delete;


	// フィルタ係数A
	float a[3];

	// フィルタ係数B
	float b[3];
};
