#pragma once
#include <string>
#include <vector>

namespace snd
{
	// �����R�[�h�ϊ�
	std::wstring ChangeCode(const std::string& code);
	std::string ChangeCode(const std::wstring& code);

	// �~����
	float PI(void);

	// ���W�A���ϊ�
	float Rad(const float& angle);

	// �n�~���O��
	float Haminng(const unsigned int& i, const size_t& num);

	// �g�`�f�[�^�̍폜
	void Delete(const std::string& fileName);
}
