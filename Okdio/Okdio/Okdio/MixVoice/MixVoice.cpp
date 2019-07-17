#include "MixVoice.h"
#include "../XAudio2/XAudio2.h"
#include "../Func/Func.h"

// �R���X�g���N�^
MixVoice::MixVoice()
{
}

// �f�X�g���N�^
MixVoice::~MixVoice()
{
}

// ������
long MixVoice::Initialize(const unsigned short& sample, const unsigned char& channel)
{
	auto hr = XAudio2::Get().Audio()->CreateSubmixVoice(&voice, channel, sample);
	if (FAILED(hr))
	{
		okmonn::DebugStream("�T�u�~�b�N�X�{�C�X�����F���s");
	}

	return hr;
}

// ���批����`�擾
XAUDIO2_SEND_DESCRIPTOR MixVoice::Send(void) const
{
	return { XAUDIO2_SEND_USEFILTER, voice };
}
