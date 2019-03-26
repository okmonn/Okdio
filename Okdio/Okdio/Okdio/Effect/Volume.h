#pragma once
#include "Effect.h"

class Volume
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
	bool SetLevel(const float& level);

	// 代入演算子オーバーロード
	void operator=(const Volume& vol);

private:
	// 初期化
	void Init(void);

	// 実行
	void Execution(std::vector<float>& data);


	// 音量レベル
	float level;
};
