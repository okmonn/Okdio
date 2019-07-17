#pragma once
#include "../Okdio.h"
#include <unknwn.h>

class CUnknown :
	public IUnknown
{
public:
	// �R���X�g���N�^
	CUnknown() : ref(1) {}
	// �f�X�g���N�^
	~CUnknown() {}

	// �Q�ƃJ�E���g���Z
	unsigned long __stdcall AddRef(void)
	{
		return InterlockedIncrement(&ref);
	}

	// �Q�ƃJ�E���g����
	unsigned long __stdcall Release(void)
	{
		unsigned long hr = InterlockedDecrement(&ref);
		if (hr == 0)
		{
			delete this;
		}

		return hr;
	}

	// �T�|�[�g�`�F�b�N
	long __stdcall QueryInterface(const GUID& id, void** obj)
	{
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}

		*obj = nullptr;
		if (id == IID_IUnknown || id == __uuidof(Okdio))
		{
			*obj = (void*)this;
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

private:
	// �Q�ƃJ�E���g
	unsigned long ref;
};
