#pragma once

struct IXAudio2SourceVoice;

namespace snd
{
	// ローパスフィルタ処理
	template <typename T>
	long LowPassFilter(T** voice, const float& cutoff, const float& q);

	// ハイパスフィルタ処理
	template <typename T>
	long HighPassFilter(T** voice, const float& cutoff, const float& q);

	// バンドパスフィルタ処理
	template <typename T>
	long BandPassFilter(T** voice, const float& cutoff, const float& q);

	// ノッチフィルタ処理
	template <typename T>
	long NotchFilter(T** voice, const float& cutoff, const float& q);
}
