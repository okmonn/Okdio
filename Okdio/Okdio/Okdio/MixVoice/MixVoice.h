#pragma once
#include "../Voice/Voice.h"

class Okdio;

class MixVoice :
	public Voice<IXAudio2SubmixVoice>
{
	friend Okdio;
public:
	// �R���X�g���N�^
	MixVoice(const unsigned char& channel = 2, const unsigned short& sample = 44100);
	// �R�s�[�R���X�g���N�^
	MixVoice(const MixVoice& mix);
	// �f�X�g���N�^
	~MixVoice();

	// ������Z�q�I�[�o�[���[�h
	void operator=(const MixVoice& mix);

private:
	// �T�u�~�b�N�X�{�C�X����
	void CreateVoice(const unsigned char& channel, const unsigned short& sample);

	// ���批����`�擾
	XAUDIO2_SEND_DESCRIPTOR GetSend(void) const;
};
