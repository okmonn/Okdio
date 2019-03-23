#pragma once

namespace snd
{
	// �T�E���h���
	struct Info
	{
	public:
		// �T���v�����O���g��
		unsigned int sample;

		// �ʎq���r�b�g��
		unsigned int bit;

		// �`�����l����
		unsigned int channel;


		// �R���X�g���N�^
		Info() {
			sample  = 0;
			bit     = 0;
			channel = 0;
		}
		Info(const unsigned int& i) {
			sample  = i;
			bit     = i;
			channel = i;
		}
		Info(const unsigned int& sample, const unsigned int& bit, const unsigned int& channel) {
			this->sample  = sample;
			this->bit     = bit;
			this->channel = channel;
		}
		Info(const Info& info) {
			sample  = info.sample;
			bit     = info.bit;
			channel = info.channel;
		}
		// �f�X�g���N�^
		~Info() {
		}

		void operator=(const unsigned int& i) {
			sample  = i;
			bit     = i;
			channel = i;
		}
		void operator=(const Info& info) {
			sample  = info.sample;
			bit     = info.bit;
			channel = info.channel;
		}
	};
}
