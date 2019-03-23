#include "XAudio2.h"
#include <xaudio2.h>

#pragma comment(lib, "xaudio2.lib")

// コンストラクタ
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
	Init();
}

// デストラクタ
XAudio2::~XAudio2()
{
	if (mastering != nullptr)
	{
		mastering->DestroyVoice();
	}

	if (audio != nullptr)
	{
		audio->Release();
	}

	CoUninitialize();
}

// COMの初期化
long XAudio2::ComInit(void)
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugStringA("\nCOMの初期化：失敗\n");
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
		OutputDebugStringA("\nオーディオ生成：失敗\n");
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
		OutputDebugStringA("\nマスタリング生成：失敗\n");
	}
#endif

	return hr;
}

// 初期化
void XAudio2::Init(void)
{
	ComInit();
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


