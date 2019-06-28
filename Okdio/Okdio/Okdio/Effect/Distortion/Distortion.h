#pragma once
#include "..//Effect.h"

namespace snd
{
	// ディストーションパラメータ
	struct DistortionParam
	{
		//増幅率
		float gain;
		//音量レベル
		float level;

		//コンストラクタ
		DistortionParam() : 
			gain(1.0f), level(1.0f){
		}
		DistortionParam(const float& gain, const float& level) :
			gain(gain), level(level) {
		}
	};
}

class __declspec(uuid("{12345678-1234-1234-1234-123456789ABC}"))
Distortion : 
	public Effect<Distortion, snd::DistortionParam>
{
public:
	// コンストラクタ
	Distortion();
	// デストラクタ
	~Distortion();

private:
	// サウンドエフェクト処理
	void DoProcess(const snd::DistortionParam& param, _Inout_updates_all_(fream* channel)
		float* __restrict data, unsigned __int32 fream, unsigned __int32 channel);
};
