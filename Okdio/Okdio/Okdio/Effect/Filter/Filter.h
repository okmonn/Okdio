#pragma once
#include "../Effect.h"
#include <functional>
#include <unordered_map>

namespace snd
{
	// フィルタタイプ
	enum class FilterType
	{
		LowPass,
		HighPass,
		BandPass,
	};
}

// デジタルフィルタ
class Filter : 
	public Effect
{
public:
	// コンストラクタ
	Filter();
	Filter(const snd::FilterType& type, const float& cutoff, const float& q, const unsigned short& sample = 44100);
	// コピーコンストラクタ
	Filter(const Filter& filter);
	// デストラクタ
	~Filter();

	// ローパスフィルタ
	// cutoff　最小値：10.0f、最大値：sampleの半分未満（10の位から切り捨てを行っています）
	// q　0.0fより大きい数値に設定してください
	// sample　適応させる波形のサンプリング周波数
	bool LowPass(const float& cutoff, const float& q, const unsigned short& sample = 44100);

	// ハイパスフィルタ
	// cutoff　最小値：10.0f、最大値：sampleの半分未満（10の位から切り捨てを行っています）
	// q　0.0fより大きい数値に設定してください
	// sample　適応させる波形のサンプリング周波数
	bool HighPass(const float& cutoff, const float& q, const unsigned short& sample = 44100);

	// バンドパスフィルタ
	// cutoff　最小値：10.0f、最大値：sampleの半分未満（10の位から切り捨てを行っています）
	// bw　0.0fより大きい数値に設定してください
	// sample　適応させる波形のサンプリング周波数
	bool BandPass(const float& cutoff, const float& bw, const unsigned short& sample = 44100);

	// 代入演算子オーバーロード
	void operator=(const Filter& filter);

private:
	// 初期化
	void Init(void);

	// 関数ポインタ初期化
	void InitFunc(void);

	// 実行
	void Execution(Okdio* okdio);


	// フィルタ係数
	float a[3];

	// フィルタ係数
	float b[3];

	// 関数ポインタ
	std::unordered_map<snd::FilterType, std::function<bool(const float&, const float&, const unsigned short&)>>func;
};
