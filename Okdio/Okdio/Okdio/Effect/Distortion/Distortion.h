#pragma once
#include "../Effect.h"

// ディストーション調節
class Distortion :
	public Effect
{
public:
	// コンストラクタ
	Distortion();
	Distortion(const float& amp);
	// コピーコンストラクタ
	Distortion(const Distortion& dis);
	// デストラクタ
	~Distortion();

	// 増幅率セット
	// amp　最小値：1.0f
	bool SetAmp(const float& amp);

	// 代入演算子オーバーロード
	void operator=(const Distortion& dis);

private:
	// 初期化
	void Init(void);

	// 実行
	void Execution(Okdio* okdio);


	// 増幅率
	float amp;
};
