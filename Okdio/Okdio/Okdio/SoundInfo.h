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

	// デジタルフィルタパラメータ
	struct FilterParam {
		//カットオフ周波数
		float cutoff;
		//帯域幅・クオリティファクタ
		float bw;
	};

	// 圧縮系パラメータ
	struct CompParam {
		//閾値
		float threshold;
		//レシオ
		float ratio;
	};

	// エフェクトタイプ
	enum Effect {
		//ローパス
		LowPass,
		//ハイパス
		HighPass,
		//バンドパス
		BandPass,
		//リミッター
		Limiter,
		//コンプレッサー
		Comp,
		//ボリューム
		Volume,
	};
}
