#pragma once
#include <string>

struct _GUID;
typedef _GUID GUID;
class Okdio;

namespace okmonn
{
	// デバッグ出力
	void DebugStream(const std::string& str);

	// エンジン開始
	bool EnginStart(void);

	// オブジェクト生成
	long CreateObj(const GUID& id, void** obj);

	// エンジン終了
	void EnginEnd(void);
}
