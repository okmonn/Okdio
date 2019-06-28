#pragma once
#include "../etc/Info.h"
#include "../Effect/Effect.h"
#include "../Func/Func.h"
#include <vector>
#include <crtdbg.h>
#include <xaudio2.h>

template<typename T>
class Voice
{
public:
	// ���ʒ���
	bool SetVolume(const float& rate);

	// �`�����l�����Ƃ̉��ʒ���
	bool SetLocalVolume(const float& angle);

	// �t�B���^����
	bool SetFilter(const snd::FilterType& type, const float& cutoff, const float& q = 1.0f);

	// �G�t�F�N�g�Z�b�g
	template <typename CLASS, typename PARAM>
	void SetEffect(Effect<CLASS, PARAM>* effect, const size_t& num)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		voice->GetVoiceDetails(&detail);

		std::vector<XAUDIO2_EFFECT_DESCRIPTOR>desc(num);
		XAUDIO2_EFFECT_DESCRIPTOR tmp{};
		for (size_t i = 0; i < num; ++i)
		{
			tmp.pEffect        = (IXAPO*)&effect[i];
			tmp.InitialState   = true;
			tmp.OutputChannels = detail.InputChannels;
			desc[i] = tmp;
		}

		XAUDIO2_EFFECT_CHAIN chain{ UINT32(num), desc.data() };
		auto hr = voice->SetEffectChain(&chain);
		_ASSERT(hr == S_OK);
	}

	// �G�t�F�N�g�p�����[�^�Z�b�g
	void SetEffectParam(const unsigned int& index, const void* param, const size_t& size);

protected:
	// �R���X�g���N�^
	Voice();
	// �f�X�g���N�^
	virtual ~Voice();


	T* voice;
};
