#include "MixVoice.h"
#include "../XAudio2/XAudio2.h"

// コンストラクタ
MixVoice::MixVoice(const unsigned char& channel, const unsigned short& sample)
{
	CreateVoice(channel, sample);
}

// コピーコンストラクタ
MixVoice::MixVoice(const MixVoice& mix)
{
	(*this) = mix;
}

// デストラクタ
MixVoice::~MixVoice()
{
	voice->DestroyVoice();
}

// サブミックスボイス生成
void MixVoice::CreateVoice(const unsigned char& channel, const unsigned short& sample)
{
	auto hr = XAudio2::Get().Audio()->CreateSubmixVoice(&voice, channel, sample);
	_ASSERT(hr == S_OK);
}

// 宛先音声定義取得
XAUDIO2_SEND_DESCRIPTOR MixVoice::GetSend(void) const
{
	return { XAUDIO2_SEND_USEFILTER, voice };
}

// 代入演算子オーバーロード
void MixVoice::operator=(const MixVoice& mix)
{
	XAUDIO2_VOICE_DETAILS detail{};
	voice->GetVoiceDetails(&detail);

	CreateVoice(detail.InputChannels, detail.InputSampleRate);
}
