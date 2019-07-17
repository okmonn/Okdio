#pragma once
#define XAUDIO2_HELPER_FUNCTIONS
#include <vector>
#include <unknwn.h>
#include <xaudio2.h>

template <typename CLASS, typename TYPE>
class Voice :
	public IUnknown
{
public:
	// ���ʒ���
	bool SetVolume(const float& rate)
	{
		if (voice != nullptr)
		{
			if (!(rate >= 0.0f && rate <= 1.0f))
			{
				return false;
			}

			if (FAILED(voice->SetVolume(rate)))
			{
				return false;
			}
			
			return true;
		}

		return false;
	}

	// �`�����l�����Ƃ̉��ʒ���
	bool SetLocalVolume(const float& angle)
	{
		if (voice != nullptr)
		{
			//�o�̓`�����l���������߂�
			XAUDIO2_VOICE_DETAILS detail{};
			XAudio2::Get().Mastering()->GetVoiceDetails(&detail);

			//�K���f�[�^
			std::vector<float>volume(detail.InputChannels, 0.0f);

			//���݂̃`�����l���������߂�
			voice->GetVoiceDetails(&detail);
			for (unsigned __int32 ch = 0; ch < detail.InputChannels; ++ch)
			{
				voice->GetChannelVolumes(ch, &volume[ch]);
			}

			float rad = ((angle + 90.0f) / 2) * (3.14159265f / 180.0f);
			for (size_t i = 0; i < volume.size(); i += detail.InputChannels)
			{
				volume[i]     *= std::cos(rad);
				volume[i + 1] *= std::sin(rad);
			}

			//�K��
			if (FAILED(voice->SetOutputMatrix(nullptr, detail.InputChannels, UINT32(volume.size()), volume.data())))
			{
				return false;
			}
			
			return true;
		}

		return false;
	}

	// ���[�p�X�t�B���^����
	long LowPassFilter(const float& cutoff, const float& q = 1.0f)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		(*voice)->GetVoiceDetails(&detail);

		XAUDIO2_FILTER_PARAMETERS param{};
		param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
		param.OneOverQ  = q;
		param.Type      = XAUDIO2_FILTER_TYPE::LowPassFilter;

		return (*voice)->SetFilterParameters(&param);
	}

	// �n�C�p�X�t�B���^����
	long HighPassFilter(const float& cutoff, const float& q = 1.0f)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		(*voice)->GetVoiceDetails(&detail);

		XAUDIO2_FILTER_PARAMETERS param{};
		param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
		param.OneOverQ  = q;
		param.Type      = XAUDIO2_FILTER_TYPE::HighPassFilter;

		return (*voice)->SetFilterParameters(&param);
	}

	// �o���h�p�X�t�B���^����
	long BandPassFilter(const float& cutoff, const float& q = 1.0f)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		(*voice)->GetVoiceDetails(&detail);

		XAUDIO2_FILTER_PARAMETERS param{};
		param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
		param.OneOverQ  = q;
		param.Type      = XAUDIO2_FILTER_TYPE::BandPassFilter;

		return (*voice)->SetFilterParameters(&param);
	}

	// �m�b�`�t�B���^����
	long NotchFilter(const float& cutoff, const float& q = 1.0f)
	{
		XAUDIO2_VOICE_DETAILS detail{};
		(*voice)->GetVoiceDetails(&detail);

		XAUDIO2_FILTER_PARAMETERS param{};
		param.Frequency = XAudio2CutoffFrequencyToRadians(cutoff, detail.InputSampleRate);
		param.OneOverQ  = q;
		param.Type      = XAUDIO2_FILTER_TYPE::NotchFilter;

		return (*voice)->SetFilterParameters(&param);
	}

	// �Q�ƃJ�E���g���Z
	unsigned long __stdcall AddRef(void)
	{
		return InterlockedIncrement(&ref);
	}

	// �Q�ƃJ�E���g����
	unsigned long __stdcall Release(void)
	{
		unsigned long hr = InterlockedDecrement(&ref);
		if (hr == 0)
		{
			delete this;
		}

		return hr;
	}

	// �T�|�[�g�`�F�b�N
	long __stdcall QueryInterface(const GUID& id, void** obj) 
	{
		if (obj == nullptr)
		{
			return E_INVALIDARG;
		}

		*obj = nullptr;
		if (id == IID_IUnknown || id == __uuidof(CLASS))
		{
			*obj = (void*)this;
			AddRef();
			return S_OK;
		}

		return E_NOINTERFACE;
	}

protected:
	// �R���X�g���N�^
	Voice() : voice(nullptr), ref(1) {}
	// �f�X�g���N�^
	virtual ~Voice() {}


	// �{�C�X
	TYPE* voice;

private:
	// �Q�ƃJ�E���g
	unsigned long ref;
};
