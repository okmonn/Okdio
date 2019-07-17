#pragma once
#include "../Okdio.h"

class VoiceCallback :
	public IXAudio2VoiceCallback
{
public:
	// �R���X�g���N�^
	VoiceCallback(Okdio* okdio) : okdio(okdio) {}
	// �f�X�g���N�^
	~VoiceCallback() {}

	// �f�[�^�ǂݍ��ݑO�ɌĂяo��
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired)
	{
		okdio->Submit();
	}

	// �V�����o�b�t�@�̏����J�n���ɌĂяo��
	void __stdcall OnBufferStart(void* pBufferContext) {
		int i = 0;
	}

	// �o�b�t�@�̏����I�����ɌĂяo��
	void __stdcall OnBufferEnd(void* pBufferContext) {
		okdio->CheckEnd();
	}

	// �����̏����p�X�I�����ɌĂяo��
	void __stdcall OnVoiceProcessingPassEnd(void) {
	}

	// �A�������X�g���[���Đ��I�����ɌĂяo��
	void __stdcall OnStreamEnd(void) {
	}

	// ���[�v�I���ʒu���B���ɌĂяo��
	void __stdcall OnLoopEnd(void* pBufferContext) {
	}

	// �G���[�������ɌĂяo��
	void __stdcall OnVoiceError(void* pBufferContext, long Error) {
	}

private:
	// �T�E���h���C�u����
	Okdio* okdio;
};
