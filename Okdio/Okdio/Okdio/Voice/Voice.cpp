#include "Voice.h"
#include "../XAudio2/XAudio2.h"
#include "../Filter/Filter.h"
#include <functional>
#include <unordered_map>

#pragma comment(lib, "XAPOBase.lib")

namespace
{
	template <typename T>
	std::unordered_map<snd::FilterType, std::function<long(T** voice, const float& cutoff, const float& q)>>filter = {
		{ snd::FilterType::LowPass,  &snd::LowPassFilter<T>  },
		{ snd::FilterType::HighPass, &snd::HighPassFilter<T> },
		{ snd::FilterType::BandPass, &snd::BandPassFilter<T> },
		{ snd::FilterType::Notch,    &snd::NotchFilter<T>    }
	};
}

// コンストラクタ
template<typename T>
Voice<T>::Voice() : 
	voice(nullptr)
{
}
template Voice<IXAudio2SubmixVoice>::Voice();
template Voice<IXAudio2SourceVoice>::Voice();

// デストラクタ
template<typename T>
Voice<T>::~Voice()
{
}
template Voice<IXAudio2SubmixVoice>::~Voice();
template Voice<IXAudio2SourceVoice>::~Voice();

// 音量調節
template<typename T>
bool Voice<T>::SetVolume(const float& rate)
{
	if (!(rate >= 0.0f && rate <= 1.0f))
	{
		return false;
	}

	auto hr = voice->SetVolume(rate);
	_ASSERT(hr == S_OK);

	return true;
}
template bool Voice<IXAudio2SubmixVoice>::SetVolume(const float&);
template bool Voice<IXAudio2SourceVoice>::SetVolume(const float&);

// チャンネルごとの音量調節
template<typename T>
bool Voice<T>::SetLocalVolume(const float& angle)
{
	//出力チャンネル数を求める
	XAUDIO2_VOICE_DETAILS detail{};
	XAudio2::Get().Mastering()->GetVoiceDetails(&detail);

	//適応データ
	std::vector<float>volume(detail.InputChannels, 0.0f);

	//現在のチャンネル数を求める
	voice->GetVoiceDetails(&detail);
	for (unsigned __int32 ch = 0; ch < detail.InputChannels; ++ch)
	{
		voice->GetChannelVolumes(ch, &volume[ch]);
	}

	for (size_t i = 0; i < volume.size(); i += detail.InputChannels)
	{
		volume[i] *= std::cos(snd::Rad((angle + 90.0f) / 2));
		volume[i + 1] *= std::sin(snd::Rad((angle + 90.0f) / 2));
	}

	//適応
	auto hr = voice->SetOutputMatrix(nullptr, detail.InputChannels, UINT32(volume.size()), volume.data());
	_ASSERT(hr == S_OK);

	return true;
}
template bool Voice<IXAudio2SubmixVoice>::SetLocalVolume(const float&);
template bool Voice<IXAudio2SourceVoice>::SetLocalVolume(const float&);

// フィルタ調節
template<typename T>
bool Voice<T>::SetFilter(const snd::FilterType& type, const float& cutoff, const float& q)
{
	if (cutoff < 0.0f)
	{
		return false;
	}

	if (!(q > 0.0f && q <= XAUDIO2_MAX_FILTER_ONEOVERQ))
	{
		return false;
	}

	auto hr = filter<T>[type](&voice, cutoff, q);
	_ASSERT(hr == S_OK);

	return true;
}
template bool Voice<IXAudio2SubmixVoice>::SetFilter(const snd::FilterType&, const float&, const float&);
template bool Voice<IXAudio2SourceVoice>::SetFilter(const snd::FilterType&, const float&, const float&);

// エフェクトパラメータセット
template<typename T>
void Voice<T>::SetEffectParam(const unsigned int& index, const void* param, const size_t& size)
{
	auto hr = voice->SetEffectParameters(index, param, UINT32(size));
	_ASSERT(hr == S_OK);
}
template void Voice<IXAudio2SubmixVoice>::SetEffectParam(const unsigned int&, const void*, const size_t&);
template void Voice<IXAudio2SourceVoice>::SetEffectParam(const unsigned int&, const void*, const size_t&);