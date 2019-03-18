#pragma once
#include <vector>

// 圧縮系エフェクト
class Comp
{
public:
	// コンストラクタ
	Comp();
	// デストラクタ
	~Comp();

	// リミッター
	void Limiter(const float& threshold, std::vector<float>& data);

	// コンプレッサー
	void Compressor(const float& threshold, const float& ratio, std::vector<float>& data);

private:
	Comp(const Comp&) = delete;
	void operator=(const Comp&) = delete;
};
