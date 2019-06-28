#pragma once

namespace snd
{
	// サウンド情報
	struct Info
	{
		//サンプリング周波数
		unsigned short sample;
		//量子化ビット数
		unsigned char bit;
		//チャンネル数
		unsigned char channel;

		//コンストラクタ
		Info() : sample(0), bit(0), channel(0) {}
		Info(const unsigned short& s, const unsigned char& b, const unsigned char& c)
			: sample(s), bit(b), channel(c) {}
	};

	// フィルタタイプ
	enum class FilterType
	{
		LowPass,
		HighPass,
		BandPass,
		Notch
	};
}
