#include "XAudio2.h"
#include <crtdbg.h>
#include <xaudio2.h>

#pragma comment(lib, "xaudio2.lib")

// �R���X�g���N�^
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
	InitCom();
	CreateAudio();
	CreateMastering();
}

// �f�X�g���N�^
XAudio2::~XAudio2()
{
	mastering->DestroyVoice();
	audio->StopEngine();
	audio->Release();
	CoUninitialize();
}

// COM������
void XAudio2::InitCom(void)
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	_ASSERT(hr == S_OK);
}

// �I�[�f�B�I����
void XAudio2::CreateAudio(void)
{
	auto hr = XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	_ASSERT(hr == S_OK);

#ifdef _DEBUG
	XAUDIO2_DEBUG_CONFIGURATION debug{};
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	audio->SetDebugConfiguration(&debug);
#endif
}

// �}�X�^�����O�{�C�X�̐���
void XAudio2::CreateMastering(void)
{
	auto hr = audio->CreateMasteringVoice(&mastering, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, nullptr, nullptr);
	_ASSERT(hr == S_OK);
}

// �I�[�f�B�I�擾
IXAudio2* XAudio2::Audio(void) const
{
	return audio;
}

// �}�X�^�����O�{�C�X�擾
IXAudio2MasteringVoice* XAudio2::Mastering(void) const
{
	return mastering;
}
