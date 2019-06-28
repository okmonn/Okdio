#pragma once
#include "..//Effect.h"

namespace snd
{
	// �f�B�X�g�[�V�����p�����[�^
	struct DistortionParam
	{
		//������
		float gain;
		//���ʃ��x��
		float level;

		//�R���X�g���N�^
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
	// �R���X�g���N�^
	Distortion();
	// �f�X�g���N�^
	~Distortion();

private:
	// �T�E���h�G�t�F�N�g����
	void DoProcess(const snd::DistortionParam& param, _Inout_updates_all_(fream* channel)
		float* __restrict data, unsigned __int32 fream, unsigned __int32 channel);
};
