#pragma once
#include <string>
#include <vector>
#include <complex>

namespace snd
{
	// �T�E���h�f�[�^�폜
	void Delete(const std::string& fileName);

	// �C�ӂ̌�����؂�̂�
	float Floor(const float& input, const int& digit);

	// �~�����擾
	inline constexpr float PI(void);

	// �n�j���O��
	template <typename T>
	T Haninng(unsigned int& i, size_t& num);

	// ���U�t�[���G�ϊ�
	std::vector<std::complex<float>> DFT(const std::vector<float>& data);
	std::vector<std::complex<double>> DFT(const std::vector<double>& data);

	// �t���U�t�[���G�ϊ�
	std::vector<float> IDFT(const std::vector<std::complex<float>>& arg);
	std::vector<double> IDFT(const std::vector<std::complex<double>>& arg);

	// �����t�[���G�ϊ�
	std::vector<std::complex<float>> FFT(const std::vector<float>& data);
	std::vector<std::complex<double>> FFT(const std::vector<double>& data);

	// �t�����t�[���G�ϊ�
	std::vector<float> IFFT(const std::vector<std::complex<float>>& data, const int& num = -1);
	std::vector<double> IFFT(const std::vector<std::complex<double>>& data, const int& num = -1);
}
