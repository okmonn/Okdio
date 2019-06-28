#pragma once
#include <string>
#include <vector>

namespace snd
{
	// 文字コード変換
	std::wstring ChangeCode(const std::string& code);
	std::string ChangeCode(const std::wstring& code);

	// 円周率
	float PI(void);

	// ラジアン変換
	float Rad(const float& angle);

	// ハミング窓
	float Haminng(const unsigned int& i, const size_t& num);

	// 波形データの削除
	void Delete(const std::string& fileName);
}
