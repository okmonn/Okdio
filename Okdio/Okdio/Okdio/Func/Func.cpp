#include "Func.h"
#include "../XAudio2/XAudio2.h"
#include "../Manager/Manager.h"
#include "../Loader/Loader.h"
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
long okmonn::EnginStart(void)
{
	return XAudio2::Get().EnginStart();
}

// �G���W���I��
void okmonn::EnginEnd(void)
{
	XAudio2::Get().EnginEnd();
}

// �I�u�W�F�N�g����
long okmonn::CreateObj(const GUID& id, void** obj)
{
	return Manager::Get().CreateObj(id, obj);
}

// �g�`�f�[�^�폜
void okmonn::Delete(const std::string& fileName)
{
	Loader::Get().Delete(fileName);
}

