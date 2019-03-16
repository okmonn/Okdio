#pragma once

// �T�E���h���֌W
namespace snd
{
	// �T�E���h���
	struct Info {
		// �T���v�����O���g��
		unsigned int sample;
		// �ʎq���r�b�g��
		unsigned int bit;
		// �`�����l����
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

	// �G�t�F�N�g�^�C�v
	enum Effect {
		//���[�p�X
		LowPass,
	};
}
