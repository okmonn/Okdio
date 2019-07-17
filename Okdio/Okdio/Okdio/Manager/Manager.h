#pragma once
#include "../etc/Singleton.h"

struct _GUID;
typedef _GUID GUID;

class Manager : 
	public Singleton<Manager>
{
	friend Singleton<Manager>;
public:
	// �I�u�W�F�N�g����
	long CreateObj(const GUID& id, void** obj);

private:
	// �R���X�g���N�^
	Manager();
	// �f�X�g���N�^
	~Manager();
};
