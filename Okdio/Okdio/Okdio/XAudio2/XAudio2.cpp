#include "XAudio2.h"
#include <xaudio2.h>
#include <tchar.h>

#pragma comment(lib, "xaudio2.lib")

// ���������
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }
#define Destroy(X) { if((X) != nullptr) (X)->DestroyVoice(); (X) = nullptr; }

// �R���X�g���N�^
XAudio2::XAudio2() :
	audio(nullptr), mastering(nullptr)
{
	Init();
}

// �f�X�g���N�^
XAudio2::~XAudio2()
{
	Destroy(mastering);
	Release(audio);
	CoUninitialize();
}

// COM������
long XAudio2::InitCom(void) const
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
#ifdef _DEBUG
	if (FAILED(hr))
	{
		OutputDebugString(_T("\nCOM�������F���s\n"));
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
		OutputDebugString(_T("\n�I�[�f�B�I�����F���s\n"));
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
		OutputDebugString(_T("\n�}�X�^�����O�����F���s\n"));
	}
#endif

	return hr;
}

// ������
void XAudio2::Init(void)
{
	InitCom();
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

// �}�X�^�����O�擾
IXAudio2MasteringVoice* XAudio2::Mastering(void) const
{
	return mastering;
}
