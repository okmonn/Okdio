#pragma once

namespace snd
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

		//�R���X�g���N�^
		Info() : sample(0), bit(0), channel(0) {}
		Info(const unsigned short& s, const unsigned char& b, const unsigned char& c)
			: sample(s), bit(b), channel(c) {}
	};

	// �t�B���^�^�C�v
	enum class FilterType
	{
		LowPass,
		HighPass,
		BandPass,
		Notch
	};
}
