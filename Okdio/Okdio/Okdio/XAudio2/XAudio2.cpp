#include "XAudio2.h"
#include <xaudio2.h>
#include <tchar.h>

#pragma comment(lib, "xaudio2.lib")

// メモリ解放
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }

// コンストラクタ
XAudio2::XAudio2() :
	audio(nullptr), mastering(nullptr)
{
	Init();
}

// デストラクタ
XAudio2::~XAudio2()
{
	Destroy(mastering);
	Release(audio);
	CoUninitialize();
}

// COM初期化
long XAudio2::InitCom(void) const
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nCOM初期化：失敗\n"));
	}
#endif

	return hr;
}

// オーディオ生成
long XAudio2::CreateAudio(void)
{
	auto hr = XAudio2Create(&audio);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nオーディオ生成：失敗\n"));
	}
#endif

	return hr;
}

// マスタリング生成
long XAudio2::CreateMastering(void)
{
	auto hr = audio->CreateMasteringVoice(&mastering);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nマスタリング生成：失敗\n"));
	}
#endif

	return hr;
}

// 初期化
void XAudio2::Init(void)
{
	InitCom();
	CreateAudio();
	CreateMastering();
}

// インスタンス変数取得
XAudio2& XAudio2::Get(void)
{
	static XAudio2 instance;
	return instance;
}

// オーディオ取得
IXAudio2* XAudio2::Audio(void) const
{
	return audio;
}

// マスタリング取得
IXAudio2MasteringVoice* XAudio2::Mastering(void) const
{
	return mastering;
}
