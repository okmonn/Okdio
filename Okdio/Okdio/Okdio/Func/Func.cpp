#include "Func.h"
#include "../XAudio2/XAudio2.h"
#include "../Manager/Manager.h"
#include <windows.h>

// デバッグ出力
void okmonn::DebugStream(const std::string& str)
{
#ifdef _DEBUG
	std::string tmp = str;
	tmp.push_back('\n');
	OutputDebugStringA(tmp.data());
#endif
}

// エンジン開始
bool okmonn::EnginStart(void)
{
	Manager::Get().Malloc();
	return XAudio2::Get().EnginStart();
}

// オブジェクト生成
long okmonn::CreateObj(const GUID& id, void** obj)
{
	return Manager::Get().Create(id, obj);
}

// エンジン終了
void okmonn::EnginEnd(void)
{
	Manager::Get().Release();
	XAudio2::Get().EnginEnd();
}
