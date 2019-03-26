#pragma once

namespace snd
{
	// サウンド情報
	struct Info
	{
	public:
		// サンプリング周波数
		unsigned short sample;

		// 量子化ビット数
		unsigned short bit;

		// チャンネル数
		unsigned short channel;


		// コンストラクタ
		Info() {
			sample  = 0;
			bit     = 0;
			channel = 0;
		}
		Info(const unsigned short& i) {
			sample  = i;
			bit     = i;
			channel = i;
		}
		Info(const unsigned short& sample, const unsigned short& bit, const unsigned short& channel) {
			this->sample  = sample;
			this->bit     = bit;
			this->channel = channel;
		}
		Info(const Info& info) {
			sample  = info.sample;
			bit     = info.bit;
			channel = info.channel;
		}
		// デストラクタ
		~Info() {
		}

		void operator=(const unsigned short& i) {
			sample  = i;
			bit     = i;
			channel = i;
		}
		void operator=(const Info& info) {
			sample  = info.sample;
			bit     = info.bit;
			channel = info.channel;
		}
	};
}
