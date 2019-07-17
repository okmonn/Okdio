#pragma once

namespace okmonn
{
	// �T�E���h���
	struct Info
	{
		//�T���v�����O���g��
		unsigned short sample;
		//�ʎq���r�b�g��
		unsigned char bit;
		//�`�����l����
		unsigned char channel;

		Info() : sample(0), bit(0), channel(0) {}
		Info(const Info& info) {
			(*this) = info;
		}
		Info(const unsigned short& s, const unsigned char& b, const unsigned char& c) :
			sample(s), bit(b), channel(c) {}
		void operator=(const Info& info) {
			sample  = info.sample;
			bit     = info.bit;
			channel = info.channel;
		}
	};
}
