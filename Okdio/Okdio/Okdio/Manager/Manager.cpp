#include "Manager.h"
#include "../Okdio.h"

// �R���X�g���N�^
Manager::Manager()
{
}

// �f�X�g���N�^
Manager::~Manager()
{
}

// �I�u�W�F�N�g����
long Manager::CreateObj(const GUID& id, void** obj)
{
	if (obj == nullptr)
	{
		return E_INVALIDARG;
	}

	*obj = nullptr;
	if (id == __uuidof(Okdio))
	{
		*obj = new Okdio();
		return S_OK;
	}

	return E_NOINTERFACE;
}
