#define XAUDIO2_HELPER_FUNCTIONS
#include "Filter.h"
#include <xaudio2.h>

template <typename T>
long snd::LowPassFilter(T** voice, const float& cutoff, const float& q)
{
	XAUDIO2_VOICE_DETAILS detail{};
	(*voice)->GetVoiceDetails(&detail);

	XAUDIO2_FILTER_PARAMETERS param{};
	param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
	param.OneOverQ  = q;
	param.Type      = XAUDIO2_FILTER_TYPE::LowPassFilter;

	return (*voice)->SetFilterParameters(&param);
}
template long snd::LowPassFilter(IXAudio2SubmixVoice**, const float&, const float&);
template long snd::LowPassFilter(IXAudio2SourceVoice**, const float&, const float&);

// ハイパスフィルタ処理
template <typename T>
long snd::HighPassFilter(T** voice, const float& cutoff, const float& q)
{
	XAUDIO2_VOICE_DETAILS detail{};
	(*voice)->GetVoiceDetails(&detail);

	XAUDIO2_FILTER_PARAMETERS param{};
	param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
	param.OneOverQ  = q;
	param.Type      = XAUDIO2_FILTER_TYPE::HighPassFilter;

	return (*voice)->SetFilterParameters(&param);
}
template long snd::HighPassFilter(IXAudio2SubmixVoice**, const float&, const float&);
template long snd::HighPassFilter(IXAudio2SourceVoice**, const float&, const float&);

// バンドパスフィルタ処理
template <typename T>
long snd::BandPassFilter(T** voice, const float& cutoff, const float& q)
{
	XAUDIO2_VOICE_DETAILS detail{};
	(*voice)->GetVoiceDetails(&detail);

	XAUDIO2_FILTER_PARAMETERS param{};
	param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
	param.OneOverQ  = q;
	param.Type      = XAUDIO2_FILTER_TYPE::BandPassFilter;

	return (*voice)->SetFilterParameters(&param);
}
template long snd::BandPassFilter(IXAudio2SubmixVoice**, const float&, const float&);
template long snd::BandPassFilter(IXAudio2SourceVoice**, const float&, const float&);

// ノッチフィルタ処理
template <typename T>
long snd::NotchFilter(T** voice, const float& cutoff, const float& q)
{
	XAUDIO2_VOICE_DETAILS detail{};
	(*voice)->GetVoiceDetails(&detail);

	XAUDIO2_FILTER_PARAMETERS param{};
	param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
	param.OneOverQ  = q;
	param.Type      = XAUDIO2_FILTER_TYPE::NotchFilter;

	return (*voice)->SetFilterParameters(&param);
}
template long snd::NotchFilter(IXAudio2SubmixVoice**, const float&, const float&);
template long snd::NotchFilter(IXAudio2SourceVoice**, const float&, const float&);
