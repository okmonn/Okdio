#pragma once
#include "../Voice/Voice.h"

class Okdio;

class MixVoice :
	public Voice<IXAudio2SubmixVoice>
{
	friend Okdio;
public:
	// コンストラクタ
	MixVoice(const unsigned char& channel = 2, const unsigned short& sample = 44100);
	// コピーコンストラクタ
	MixVoice(const MixVoice& mix);
	// デストラクタ
	~MixVoice();

	// 代入演算子オーバーロード
	void operator=(const MixVoice& mix);

private:
	// サブミックスボイス生成
	void CreateVoice(const unsigned char& channel, const unsigned short& sample);

	// 宛先音声定義取得
	XAUDIO2_SEND_DESCRIPTOR GetSend(void) const;
};
