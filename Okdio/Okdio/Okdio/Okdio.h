#pragma once
#include "SoundInfo.h"
#include <string>
#include <vector>
#include <optional>
#include <xaudio2.h>

class Okdio :
	public IXAudio2VoiceCallback
{
public:
	// �R���X�g���N�^
	Okdio();
	Okdio(const std::string& fileName);
	// �R�s�[�R���X�g���N�^
	Okdio(const Okdio& okdio);
	// �f�X�g���N�^
	~Okdio();

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

	// ���
	void operator=(const Okdio& okdio);

private:
	// ������
	void Init(void);

	// �\�[�X�{�C�X����
	long CreateVoice(void);

	// �g�`�f�[�^�X�V
	void UpData(void);

	// �o�b�t�@�ɒǉ�
	long Submit(void);

	// �I���m�F
	void CheckEnd(void);

	// �g�`�ǂݍ��݂̃��Z�b�g
	void Reset(void);

	// ���̏����f�[�^�擾
	inline constexpr unsigned int Bps(void) const;

	// �f�[�^�ǂݍ��ݑO�ɌĂяo��
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired);
	// �V�����o�b�t�@�̏����J�n���ɌĂяo��
	void __stdcall OnBufferStart(void* pBufferContext);
	// �o�b�t�@�̏����I�����ɌĂяo��
	void __stdcall OnBufferEnd(void* pBufferContext);
	// �����̏����p�X�I�����ɌĂяo��
	void __stdcall OnVoiceProcessingPassEnd();
	// �A�������X�g���[���Đ��I�����ɌĂяo��
	void __stdcall OnStreamEnd() {}
	// ���[�v�I���ʒu���B���ɌĂяo��
	void __stdcall OnLoopEnd(void* pBufferContext) {}
	// �G���[�������ɌĂяo��
	void __stdcall OnVoiceError(void* pBufferContext, long Error) {}


	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �Q�ƃt�@�C����
	std::optional<std::string>name;

	// �o�b�t�@����ւ��p�C���f�b�N�X
	unsigned int index;

	// 
	unsigned int cnt;

	// ���[�v�t���O
	bool loop;

	// �G�t�F�N�g���������n���h��
	void* handle;

	// �T�E���h���
	snd::Info info;

	// �ǂݍ��݈ʒu
	std::vector<unsigned int>read;

	// �g�`�f�[�^
	std::vector<std::vector<float>>wave;
};
