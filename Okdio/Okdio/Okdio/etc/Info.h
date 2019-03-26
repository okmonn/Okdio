#pragma once

namespace snd
{
	// �T�E���h���
	struct Info
	{
	public:
		// �T���v�����O���g��
		unsigned short sample;

		// �ʎq���r�b�g��
		unsigned short bit;

		// �`�����l����
		unsigned short channel;


		// �R���X�g���N�^
		Info() {
			sample  = 0;
			bit     = 0;
			channel = 0;
		}
		Info(const unsigned short& i) {
			sample  = i;
			bit     = i;
			channel = i;
		}
		Info(const unsigned short& sample, const unsigned short& bit, const unsigned short& channel) {
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

		void operator=(const unsigned short& i) {
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
