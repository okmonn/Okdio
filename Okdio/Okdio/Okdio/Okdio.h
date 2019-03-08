#pragma once
#include "SoundInfo.h"
#include <string>
#include <vector>
#include <optional>
#include <xaudio2.h>

// �T�E���h
class Okdio : 
	public IXAudio2VoiceCallback
{
public:
	// �R���X�g���N�^
	Okdio(const std::string& fileName);
	// �f�X�g���N�^
	~Okdio();

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

private:
	// ������
	void Init(void);

	// �\�[�X�{�C�X����
	long CreateVoice(void);

	// �o�b�t�@�ɒǉ�
	long Submit(void);

	// �I���m�F
	void CheckEnd(void);

	// �V�����o�b�t�@�̏����J�n���ɌĂяo��
	void __stdcall OnBufferStart(void* pBufferContext);
	// �o�b�t�@�̏����I�����ɌĂяo��
	void __stdcall OnBufferEnd(void* pBufferContext);
	// �f�[�^�ǂݍ��ݑO�ɌĂяo��
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired);
	// �����̏����p�X�I�����ɌĂяo��
	void __stdcall OnVoiceProcessingPassEnd();
	// �A�������X�g���[���Đ��I�����ɌĂяo��
	void __stdcall OnStreamEnd();
	// ���[�v�I���ʒu���B���ɌĂяo��
	void __stdcall OnLoopEnd(void* pBufferContext);
	// �G���[�������ɌĂяo��
	void __stdcall OnVoiceError(void* pBufferContext, long Error);


	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �Q�ƃt�@�C���p�X
	std::optional<std::string>name;

	// �T�E���h���
	snd::Info info;

	// �z��p�C���f�b�N�X
	unsigned int index;

	// �Đ��Ăяo����
	unsigned int cnt;

	// ���[�v�t���O
	bool loop;

	// ���̏����f�[�^�T�C�Y
	unsigned int bps;

	// �ǂݍ��݈ʒu
	std::vector<unsigned int>read;

	// �g�`�f�[�^
	std::vector<std::vector<float>>wave;
};
