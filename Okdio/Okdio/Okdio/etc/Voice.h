#pragma once
#include <unknwn.h>
#include <xaudio2.h>

template <typename CLASS, typename TYPE>
class Voice :
	public IUnknown
{
public:
	// ���ʒ���
	bool SetVolume(const float& rate)
	{

	}

	// �`�����l�����Ƃ̉��ʒ���
	bool SetLocalVolume(const float& angle)
	{

	}

	// ���[�p�X�t�B���^����
	long LowPassFilter(const float& cutoff, const float& q = 1.0f)
	{

	}

	// �n�C�p�X�t�B���^����
	long HighPassFilter(const float& cutoff, const float& q = 1.0f)
	{

	}

	// �o���h�p�X�t�B���^����
	long BandPassFilter(const float& cutoff, const float& q = 1.0f)
	{

	}

	// �m�b�`�t�B���^����
	long NotchFilter(const float& cutoff, const float& q = 1.0f)
	{

	}

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
		if (id == IID_IUnknown || id == __uuidof(CLASS))
		{
			*obj = (void*)new CLASS();
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

protected:
	// �R���X�g���N�^
	Voice() : voice(nullptr), ref(1) {}
	// �f�X�g���N�^
	virtual ~Voice() {}


	// �{�C�X
	TYPE* voice;

private:
	// �Q�ƃJ�E���g
	unsigned long ref;
};
