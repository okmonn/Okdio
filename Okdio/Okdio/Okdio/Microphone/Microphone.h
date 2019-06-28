#pragma once
#include "../etc/Singleton.h"
#include "../etc/Info.h"
#include <array>
#include <vector>
#include <Windows.h>

class Okdio;

class Microphone :
	public Singleton<Microphone>
{
	friend Singleton<Microphone>;

	// ���͗p�\����
	struct WAVEIN
	{
		WAVEHDR hdr;
		std::vector<short>InData;

		//�R���X�g���N�^
		WAVEIN() {
			hdr = {};
		}
		WAVEIN(const snd::Info& info) : hdr({}) {
			Set(info);
		}

		//���Z�b�g
		void Set(const snd::Info& info) {
			InData.resize(info.sample * info.channel);
			hdr.dwBufferLength = (DWORD)InData.size();
			hdr.lpData = (LPSTR)InData.data();
		}
	};

public:
	// �Đ��^�[�Q�b�g�Z�b�g
	void SetAudio(Okdio* okdio);

	// ���͊J�n
	void Start(void);

private:
	// �R���X�g���N�^
	Microphone();
	// �f�X�g���N�^
	~Microphone();

	// ���̓R�[���o�b�N
	static void __stdcall WaveInProc(HWAVEIN handle, unsigned int msg, unsigned long instance, WPARAM param1, LPARAM param2);

	// ���̓f�o�C�X�I�[�v��
	void InOpen(void);

	// ���͏I��
	void InFinish(void);


	// �T�E���h���C�u����
	static Okdio* okdio;

	// ���̓n���h��
	HWAVEIN InHandle;

	// �T�E���h���
	snd::Info info;

	// �I���t���O
	static bool fin;

	// ���͗p
	static std::array<WAVEIN, 2>waveIn;

	// �Đ��p�f�[�^
	static std::vector<float>data;
};
