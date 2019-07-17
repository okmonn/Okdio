#include "Func.h"
#include "../XAudio2/XAudio2.h"
#include "../Manager/Manager.h"
#include <windows.h>

// �f�o�b�O�o��
void okmonn::DebugStream(const std::string& str)
{
#ifdef _DEBUG
	std::string tmp = str;
	tmp.push_back('\n');
	OutputDebugStringA(tmp.data());
#endif
}

// �G���W���J�n
bool okmonn::EnginStart(void)
{
	Manager::Get().Malloc();
	return XAudio2::Get().EnginStart();
}

// �I�u�W�F�N�g����
long okmonn::CreateObj(const GUID& id, void** obj)
{
	return Manager::Get().Create(id, obj);
}

// �G���W���I��
void okmonn::EnginEnd(void)
{
	Manager::Get().Release();
	XAudio2::Get().EnginEnd();
}
