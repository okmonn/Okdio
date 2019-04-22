#pragma once
#include "../Effect.h"

class TimeStretch :
	public Effect
{
public:
	// コンストラクタ
	TimeStretch();
	TimeStretch(const float& speed);
	// コピーコンストラクタ
	TimeStretch(const TimeStretch& time);
	// デストラクタ
	~TimeStretch();

	// 再生速度セット
	// speed　0.0fより大きい値
	bool SetSpeed(const float& speed);

	// 代入演算子オーバーロード
	void operator=(const TimeStretch& time);

private:
	// 初期化
	void Init(void);

	// 実行
	void Execution(Okdio* okdio);


	// 再生速度
	float speed;
};
