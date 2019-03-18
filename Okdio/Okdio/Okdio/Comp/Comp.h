#pragma once
#include <vector>

// ���k�n�G�t�F�N�g
class Comp
{
public:
	// �R���X�g���N�^
	Comp();
	// �f�X�g���N�^
	~Comp();

	// ���~�b�^�[
	void Limiter(const float& threshold, std::vector<float>& data);

	// �R���v���b�T�[
	void Compressor(const float& threshold, const float& ratio, std::vector<float>& data);

private:
	Comp(const Comp&) = delete;
	void operator=(const Comp&) = delete;
};
