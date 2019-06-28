#include "VoiceCallback.h"
#include "../Okdio.h"

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
	okdio->Submit();
}

// �V�����o�b�t�@�̏����J�n���ɌĂяo��
void __stdcall VoiceCallback::OnBufferStart(void* pBufferContext)
{
}

// �o�b�t�@�̏����I�����ɌĂяo��
void __stdcall VoiceCallback::OnBufferEnd(void* pBufferContext)
{
	okdio->CheckEnd();
}

// �����̏����p�X�I�����ɌĂяo��
void __stdcall VoiceCallback::OnVoiceProcessingPassEnd(void)
{
}
