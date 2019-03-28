#pragma once
#include "../Effect.h"

namespace snd
{
	// フィルタタイプ
	enum FilterType
	{
		LowPass,
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
	bool LowPass(const float& cutoff, const float& q, const unsigned short& sample = 44100);

	// 代入演算子オーバーロード
	void operator=(const Filter& filter);

private:
	// 初期化
	void Init(void);

	// 実行
	void Execution(Okdio* okdio);


	// フィルタ係数
	float a[3];

	// フィルタ係数
	float b[3];
};
