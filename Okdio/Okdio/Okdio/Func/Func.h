#pragma once
#include <string>

struct _GUID;
typedef _GUID GUID;
class Okdio;

namespace okmonn
{
	// �f�o�b�O�o��
	void DebugStream(const std::string& str);

	// �G���W���J�n
	bool EnginStart(void);

	// �I�u�W�F�N�g����
	long CreateObj(const GUID& id, void** obj);

	// �G���W���I��
	void EnginEnd(void);
}
