#pragma once
#include <xaudio2.h>

class Okdio;

class VoiceCallback : 
	public IXAudio2VoiceCallback
{
public:
	// �R���X�g���N�^
	VoiceCallback(Okdio* okdio);
	// �f�X�g���N�^
	~VoiceCallback();

private:
	VoiceCallback(const VoiceCallback&) = delete;
	void operator=(const VoiceCallback&) = delete;

	// �f�[�^�ǂݍ��ݑO�ɌĂяo��
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired);

	// �V�����o�b�t�@�̏����J�n���ɌĂяo��
	void __stdcall OnBufferStart(void* pBufferContext);

	// �o�b�t�@�̏����I�����ɌĂяo��
	void __stdcall OnBufferEnd(void* pBufferContext);

	// �����̏����p�X�I�����ɌĂяo��
	void __stdcall OnVoiceProcessingPassEnd(void);

	// �A�������X�g���[���Đ��I�����ɌĂяo��
	void __stdcall OnStreamEnd(void) {}

	// ���[�v�I���ʒu���B���ɌĂяo��
	void __stdcall OnLoopEnd(void* pBufferContext) {}

	// �G���[�������ɌĂяo��
	void __stdcall OnVoiceError(void* pBufferContext, long Error) {}


	// �T�E���h���C�u����
	Okdio* okdio;
};
