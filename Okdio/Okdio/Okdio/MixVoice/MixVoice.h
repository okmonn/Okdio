#pragma once
#include "../etc/Voice.h"

class Manager;
class Okdio;

class __declspec(uuid("00000000-0917-0917-0917-000000000002"))
MixVoice : public Voice<MixVoice, IXAudio2SubmixVoice>
{
	friend Manager;
	friend Okdio;
public:
	// ������
	long Initialize(const unsigned short& sample, const unsigned char& channel);

private:
	// �R���X�g���N�^
	MixVoice();
	MixVoice(const MixVoice&) = delete;
	void operator=(const MixVoice&) = delete;
	// �f�X�g���N�^
	~MixVoice();

	// ���批����`�擾
	XAUDIO2_SEND_DESCRIPTOR Send(void) const;
};
