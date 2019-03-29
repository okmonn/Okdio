#pragma once
#include "../Effect.h"

// 定位調節
class Pan : 
	public Effect
{
public:
	// コンストラクタ
	Pan();
	Pan(const float& angle);
	// コピーコンストラクタ
	Pan(const Pan& pan);
	// デストラクタ
	~Pan();

	// 定位角度セット
	//angle　最小値：-90.0f、最大値：90.0f
	bool SetPanning(const float& angle);

	// 代入演算子オーバーロード
	void operator=(const Pan& pan);

private:
	// 初期化
	void Init(void);

	// 実行
	void Execution(Okdio* okdio);


	// 定位角度
	float angle;
};
