#pragma once
#include <string>
#include <vector>

namespace snd
{
	// �T�E���h�f�[�^�폜
	void Delete(const std::string& fileName);

	// �C�ӂ̌�����؂�̂�
	float Floor(const float& input, const int& digit);

	// �~�����擾
	inline constexpr float PI(void);

	// ���U�t�[���G�ϊ�
	void DFT(const std::vector<float>& data, std::vector<float>& real, std::vector<float>& imag);

	// �t���U�t�[���G�ϊ�
	std::vector<float> IDFT(const std::vector<float>& real, const std::vector<float>& imag);
}
