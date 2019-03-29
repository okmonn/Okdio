#pragma once
#include "../Effect.h"

// 音量調節
class Volume : 
	public Effect
{
public:
	// コンストラクタ
	Volume();
	Volume(const float& level);
	// コピーコンストラクタ
	Volume(const Volume& vol);
	// デストラクタ
	~Volume();

	// 音量レベルセット
	// level　最小値：0.0f、最大値：1.0f
	bool SetLevel(const float& level);

	// 代入演算子オーバーロード
	void operator=(const Volume& vol);

private:
	// 初期化
	void Init(void);

	// 実行
	void Execution(Okdio* okdio);


	// 音量レベル
	float level;
};
