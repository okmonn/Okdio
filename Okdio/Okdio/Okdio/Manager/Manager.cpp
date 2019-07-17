#include "Manager.h"
#include "../Okdio.h"

// �R���X�g���N�^
Manager::Manager() : 
	okdio(nullptr)
{
}

// �f�X�g���N�^
Manager::~Manager()
{
	Release();
}

// �������m��
void Manager::Malloc(void)
{
	if (okdio == nullptr)
	{
		okdio = new Okdio();
	}
}

// �I�u�W�F�N�g����
long Manager::Create(const GUID& id, void** obj)
{
	return okdio->QueryInterface(id, obj);
}

// �������
void Manager::Release(void)
{
	if (okdio != nullptr)
	{
		okdio->Release();
		okdio = nullptr;
	}
}
