#include "MixVoice.h"
#include "../XAudio2/XAudio2.h"

// �R���X�g���N�^
MixVoice::MixVoice(const unsigned char& channel, const unsigned short& sample)
{
	CreateVoice(channel, sample);
}

// �R�s�[�R���X�g���N�^
MixVoice::MixVoice(const MixVoice& mix)
{
	(*this) = mix;
}

// �f�X�g���N�^
MixVoice::~MixVoice()
{
	voice->DestroyVoice();
}

// �T�u�~�b�N�X�{�C�X����
void MixVoice::CreateVoice(const unsigned char& channel, const unsigned short& sample)
{
	auto hr = XAudio2::Get().Audio()->CreateSubmixVoice(&voice, channel, sample);
	_ASSERT(hr == S_OK);
}

// ���批����`�擾
XAUDIO2_SEND_DESCRIPTOR MixVoice::GetSend(void) const
{
	return { XAUDIO2_SEND_USEFILTER, voice };
}

// ������Z�q�I�[�o�[���[�h
void MixVoice::operator=(const MixVoice& mix)
{
	XAUDIO2_VOICE_DETAILS detail{};
	voice->GetVoiceDetails(&detail);

	CreateVoice(detail.InputChannels, detail.InputSampleRate);
}
