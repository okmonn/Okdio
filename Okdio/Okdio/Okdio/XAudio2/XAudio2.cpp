#include "XAudio2.h"
#include "../Func/Func.h"
#include <xaudio2.h>

// �R���X�g���N�^
XAudio2::XAudio2() : 
	audio(nullptr), mastering(nullptr)
{
}

// �f�X�g���N�^
XAudio2::~XAudio2()
{
	EnginEnd();
}

// �I�[�f�B�I����
long XAudio2::CreateAudio(void)
{
	return XAudio2Create(&audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
}

// �}�X�^�����O�{�C�X����
long XAudio2::CreateMastering(void)
{
	if (audio != nullptr)
	{
		return audio->CreateMasteringVoice(&mastering, XAUDIO2_DEFAULT_CHANNELS, XAUDIO2_DEFAULT_SAMPLERATE, 0, nullptr, nullptr);
	}

	return E_POINTER;
}

// �J�n
bool XAudio2::EnginStart(void)
{
	if (FAILED(CreateAudio()))
	{
		okmonn::DebugStream("XAudio2�̐����F���s");
		return false;
	}
	if (FAILED(CreateMastering()))
	{
		okmonn::DebugStream("�}�X�^�����O�{�C�X�̐����F���s");
		return false;
	}

#ifdef _DEBUG
	XAUDIO2_DEBUG_CONFIGURATION debug{};
	debug.BreakMask = XAUDIO2_LOG_ERRORS;
	debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	audio->SetDebugConfiguration(&debug);
#endif
	
	return true;
}

// �I��
void XAudio2::EnginEnd(void)
{
	if (mastering != nullptr)
	{
		mastering->DestroyVoice();
		mastering = nullptr;
	}
	if (audio != nullptr)
	{
		audio->Release();
		audio = nullptr;
	}
}

// �I�[�f�B�I�擾
IXAudio2* XAudio2::Audio(void) const
{
	return audio;
}
