#pragma once

struct IXAudio2SourceVoice;

namespace snd
{
	// ���[�p�X�t�B���^����
	template <typename T>
	long LowPassFilter(T** voice, const float& cutoff, const float& q);

	// �n�C�p�X�t�B���^����
	template <typename T>
	long HighPassFilter(T** voice, const float& cutoff, const float& q);

	// �o���h�p�X�t�B���^����
	template <typename T>
	long BandPassFilter(T** voice, const float& cutoff, const float& q);

	// �m�b�`�t�B���^����
	template <typename T>
	long NotchFilter(T** voice, const float& cutoff, const float& q);
}
