#include "Func.h"
#include "../Loader/Loader.h"
#include <mutex>
#include <Windows.h>
#include <algorithm>

// �����R�[�h�ϊ�
std::wstring snd::ChangeCode(const std::string& code)
{
	//�T�C�Y�̎擾
	auto byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, code.c_str(), -1, nullptr, 0);

	std::wstring wstr;
	wstr.resize(byteSize);

	//�ϊ�
	byteSize = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, code.c_str(), -1, &wstr[0], byteSize);

	return wstr;
}

// �����R�[�h�ϊ�
std::string snd::ChangeCode(const std::wstring& code)
{
	//�T�C�Y�̎擾
	auto byteSize = WideCharToMultiByte(CP_ACP, 0, code.c_str(), -1, nullptr, 0, nullptr, nullptr);

	std::string str;
	str.resize(byteSize);

	//�ϊ�
	byteSize = WideCharToMultiByte(CP_ACP, 0, code.c_str(), static_cast<int>(code.size()), &str[0], byteSize, nullptr, nullptr);

	return str;
}

// �~����
float snd::PI(void)
{
	return 3.14159265358979323846f;
}

// ���W�A���ϊ�
float snd::Rad(const float& angle)
{
	return angle * (PI() / 180.0f);
}

// �n�~���O��
float snd::Haminng(const unsigned int& i, const size_t& num)
{
	return (i < num)
		? float(0.54 - 0.46 * std::cos(2.0 * PI() * i / num))
		: 0.0f;
}

// �g�`�f�[�^�̍폜
void snd::Delete(const std::string& fileName)
{
	Loader::Get().Delete(fileName);
}
