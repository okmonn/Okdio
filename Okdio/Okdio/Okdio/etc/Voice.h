#pragma once
#include <unknwn.h>
#include <xaudio2.h>

template <typename CLASS, typename TYPE>
class Voice :
	public IUnknown
{
public:
	// 音量調節
	bool SetVolume(const float& rate)
	{

	}

	// チャンネルごとの

	// 参照カウント加算
	unsigned long __stdcall AddRef(void) {
		return InterlockedIncrement(&ref);
	}

	// 参照カウント減少
	unsigned long __stdcall Release(void) {
		unsigned long hr = InterlockedDecrement(&ref);
		if (hr == 0)
		{
			delete this;
		}

		return hr;
	}

	// サポートチェック
	long __stdcall QueryInterface(const GUID& id, void** obj) {
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
	// コンストラクタ
	Voice() : voice(nullptr), ref(1) {}
	// デストラクタ
	virtual ~Voice() {}


	// ボイス
	TYPE* voice;

private:
	// 参照カウント
	unsigned long ref;
};
