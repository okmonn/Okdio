#pragma once
#include <string>
#include <vector>
#include <complex>

namespace snd
{
	// サウンドデータ削除
	void Delete(const std::string& fileName);

	// 任意の桁から切り捨て
	float Floor(const float& input, const int& digit);

	// 円周率取得
	inline constexpr float PI(void);

	// ハニング窓
	template <typename T>
	T Haninng(unsigned int& i, size_t& num);

	// 離散フーリエ変換
	std::vector<std::complex<float>> DFT(const std::vector<float>& data);
	std::vector<std::complex<double>> DFT(const std::vector<double>& data);

	// 逆離散フーリエ変換
	std::vector<float> IDFT(const std::vector<std::complex<float>>& arg);
	std::vector<double> IDFT(const std::vector<std::complex<double>>& arg);

	// 高速フーリエ変換
	std::vector<std::complex<float>> FFT(const std::vector<float>& data);
	std::vector<std::complex<double>> FFT(const std::vector<double>& data);

	// 逆高速フーリエ変換
	std::vector<float> IFFT(const std::vector<std::complex<float>>& data, const int& num = -1);
	std::vector<double> IFFT(const std::vector<std::complex<double>>& data, const int& num = -1);
}
