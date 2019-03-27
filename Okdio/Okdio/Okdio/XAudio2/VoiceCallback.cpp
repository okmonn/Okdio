#include "VoiceCallback.h"
#include "../OKdio.h"

// �R���X�g���N�^
VoiceCallback::VoiceCallback(Okdio* okdio) : 
	okdio(okdio)
{
}

// �f�X�g���N�^
VoiceCallback::~VoiceCallback()
{
}

// �f�[�^�ǂݍ��ݑO�ɌĂяo��
void __stdcall VoiceCallback::OnVoiceProcessingPassStart(unsigned int SamplesRequired)
{
	if (okdio->effector != nullptr)
	{
		WaitForSingleObject(okdio->handle, INFINITE);
	}

	okdio->Submit();
}

// �V�����o�b�t�@�̏����J�n���ɌĂяo��
void __stdcall VoiceCallback::OnBufferStart(void * pBufferContext)
{
	okdio->UpData();
}

// �o�b�t�@�̏����I�����ɌĂяo��
void __stdcall VoiceCallback::OnBufferEnd(void * pBufferContext)
{
	okdio->CheckEnd();
}

// �����̏����p�X�I�����ɌĂяo��
void __stdcall VoiceCallback::OnVoiceProcessingPassEnd(void)
{
	okdio->Reset();
}
