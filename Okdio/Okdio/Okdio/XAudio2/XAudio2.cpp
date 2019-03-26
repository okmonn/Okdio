#include "XAudio2.h"
#include <xaudio2.h>

#pragma comment(lib, "xaudio2.lib")

// �R���X�g���N�^
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
	Init();
}

// �f�X�g���N�^
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

// COM�̏�����
long XAudio2::ComInit(void)
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugStringA("\nCOM�̏������F���s\n");
	}
#endif

	return hr;
}

// �I�[�f�B�I����
long XAudio2::CreateAudio(void)
{
	auto hr = XAudio2Create(&audio);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugStringA("\n�I�[�f�B�I�����F���s\n");
	}
#endif

	return hr;
}

// �}�X�^�����O����
long XAudio2::CreateMastering(void)
{
	auto hr = audio->CreateMasteringVoice(&mastering);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugStringA("\n�}�X�^�����O�����F���s\n");
	}
#endif

	return hr;
}

// ������
void XAudio2::Init(void)
{
	ComInit();
	CreateAudio();
	CreateMastering();
}

// �C���X�^���X�ϐ��擾
XAudio2& XAudio2::Get(void)
{
	static XAudio2 instance;
	return instance;
}

// �I�[�f�B�I�擾
IXAudio2* XAudio2::Audio(void) const
{
	return audio;
}


