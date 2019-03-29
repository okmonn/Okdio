#pragma once
#include <string>
#include <vector>

namespace snd
{
	// サウンドデータ削除
	void Delete(const std::string& fileName);

	// 任意の桁から切り捨て
	float Floor(const float& input, const int& digit);

	// 円周率取得
	inline constexpr float PI(void);

	// 離散フーリエ変換
	void DFT(const std::vector<float>& data, std::vector<float>& real, std::vector<float>& imag);

	// 逆離散フーリエ変換
	std::vector<float> IDFT(const std::vector<float>& real, const std::vector<float>& imag);
}
