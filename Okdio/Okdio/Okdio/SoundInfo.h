#pragma once

// サウンド情報関係
namespace snd
{
	// サウンド情報
	struct Info {
		// サンプリング周波数
		unsigned int sample;
		// 量子化ビット数
		unsigned int bit;
		// チャンネル数
		unsigned int channel;

		Info() {
			sample = 0;
			bit = 0;
			channel = 0;
		}
		Info(const unsigned int& sample, const unsigned int& bit, const unsigned int& channel) {
			this->sample  = sample;
			this->bit     = bit;
			this->channel = channel;
		}
		Info(const unsigned int& value) {
			sample  = value;
			bit     = value;
			channel = value;
		}
	};

	// エフェクトタイプ
	enum Effect {
		//ローパス
		LowPass,
	};
}
