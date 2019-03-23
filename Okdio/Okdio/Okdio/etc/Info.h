#pragma once

namespace snd
{
	// サウンド情報
	struct Info
	{
	public:
		// サンプリング周波数
		unsigned int sample;

		// 量子化ビット数
		unsigned int bit;

		// チャンネル数
		unsigned int channel;


		// コンストラクタ
		Info() {
			sample  = 0;
			bit     = 0;
			channel = 0;
		}
		Info(const unsigned int& i) {
			sample  = i;
			bit     = i;
			channel = i;
		}
		Info(const unsigned int& sample, const unsigned int& bit, const unsigned int& channel) {
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

		void operator=(const unsigned int& i) {
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
