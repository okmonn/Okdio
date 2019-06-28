#pragma once
#include <array>
#include <crtdbg.h>
#include <xapobase.h>

template <typename CLASS, typename PARAM>
class Effect :
	public CXAPOParametersBase
{
public:
	// 実行準備
	virtual __declspec(nothrow) long __stdcall LockForProcess(
		unsigned __int32 InputParamCnt,
		_In_reads_opt_(InputParamCnt) const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* InputParam,
		unsigned __int32 OutputParamCnt,
		_In_reads_opt_(OutputParamCnt) const XAPO_LOCKFORPROCESS_BUFFER_PARAMETERS* OutputParam) override
	{
		if (InputParam == nullptr)
		{
			return S_FALSE;
		}

		auto hr = CXAPOParametersBase::LockForProcess(InputParamCnt, InputParam, OutputParamCnt, OutputParam);
		_ASSERT(hr == S_OK);

		memcpy(&fmt, InputParam[0].pFormat, sizeof(WAVEFORMATEX));

		return hr;
	}

	// 実行
	virtual __declspec(nothrow) void __stdcall Process(
		unsigned __int32 InputParamCnt,
		_In_reads_opt_(InputParamCnt) const XAPO_PROCESS_BUFFER_PARAMETERS* InputParam,
		unsigned __int32 OutputParamCnt,
		_Inout_updates_opt_(OutputParamCnt) XAPO_PROCESS_BUFFER_PARAMETERS* OutputParam,
		int enable) override
	{
		_ASSERT(IsLocked());
		_ASSERT(InputParamCnt == 1 && OutputParamCnt == 1);
		_ASSERT(InputParam != 0 && OutputParam != 0);
		_Analysis_assume_(InputParam != 0 && OutputParam != 0);
		_ASSERT(InputParam[0].pBuffer == OutputParam[0].pBuffer);

		PARAM* pParams = (PARAM*)BeginProcess();

		switch (InputParam[0].BufferFlags)
		{
		case XAPO_BUFFER_SILENT:
		{
			memset(InputParam[0].pBuffer, 0, sizeof(float) * InputParam[0].ValidFrameCount * fmt.nChannels);
			DoProcess(*pParams, (float* __restrict)InputParam[0].pBuffer, InputParam[0].ValidFrameCount, fmt.nChannels);
			break;
		}
		case XAPO_BUFFER_VALID:
		{
			DoProcess(*pParams, (float* __restrict)InputParam[0].pBuffer, InputParam[0].ValidFrameCount, fmt.nChannels);
			break;
		}
		default:
			break;
		}

		EndProcess();
	}

protected:
	// コンストラクタ
	Effect() :
		CXAPOParametersBase(&regist, (unsigned char*)param.data(), sizeof(PARAM), 0)
	{
		memset(param.data(), 0, sizeof(param));
		Init();
	}
	// デストラクタ
	virtual ~Effect() {}

	// パラメータのセット
	virtual void OnSetParameters(const PARAM&) {}
	void OnSetParameters(_In_reads_bytes_(size) const void* param, unsigned __int32 size) override
	{
		_ASSERT(size == sizeof(PARAM));
		OnSetParameters(*(PARAM*)param);
	}

	// エフェクト処理実行
	virtual void DoProcess(const PARAM& param, _Inout_updates_all_(fream* channel) float* __restrict data,
		unsigned __int32 fream, unsigned __int32 channel) = 0;

private:
	// 初期化
	void Init(void)
	{
		auto hr = Initialize(nullptr, 0);
		_ASSERT(hr == S_OK);
	}


	// サウンド情報
	WAVEFORMATEX fmt;

	// リングバッファ
	std::array<PARAM, 3>param;

	// 登録情報
	static XAPO_REGISTRATION_PROPERTIES regist;
};

template <typename CLASS, typename PARAM>
__declspec(selectany) XAPO_REGISTRATION_PROPERTIES Effect<CLASS, PARAM>::regist = {
			__uuidof(CLASS),
			L"CustomAPO",
			L"おかもん",
			1,
			0,
			XAPOBASE_DEFAULT_FLAG,
			1, 1, 1, 1
};
