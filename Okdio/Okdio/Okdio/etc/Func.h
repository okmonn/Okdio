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

	// ���U�t�[���G�ϊ�
	void DFT(const std::vector<float>& data, std::vector<std::complex<float>>& arg);
	void DFT(const std::vector<double>& data, std::vector<std::complex<double>>& arg);

	// �t���U�t�[���G�ϊ�
	std::vector<float> IDFT(const std::vector<std::complex<float>>& arg);
	std::vector<double> IDFT(const std::vector<std::complex<double>>& arg);
}
