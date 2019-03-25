#pragma once
#include "etc/Info.h"
#include <string>
#include <vector>
#include <xaudio2.h>

// �T�E���h���C�u����
class Okdio :
	IXAudio2VoiceCallback
{
public:
	// �R���X�g���N�^
	Okdio();
	Okdio(const std::string& fileName);
	Okdio(const snd::Info& info, const std::vector<float>& data);
	// �R�s�[�R���X�g���N�^
	Okdio(const Okdio& okdio);
	// �f�X�g���N�^
	~Okdio();

	// �t�@�C������ǂݍ���
	int Load(const std::string& fileName);

	// �I���W�i���g�`����
	int CreateOriginal(const snd::Info & info, const std::vector<float>& data);

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Okdio& okdio);

private:
	// ������
	void Init(void);

	// �\�[�X�{�C�X����
	long CreateVoice(void);

	// �g�`�f�[�^���{�C�X�o�b�t�@�ɒǉ�
	long Submit(void);

	// �g�`�f�[�^�X�V
	void UpData(void);

	// �I���m�F
	void CheckEnd(void);

	// ���Z�b�g
	void Reset(void);

	// �f�[�^�ǂݍ��ݑO�ɌĂяo��
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired);
	// �V�����o�b�t�@�̏����J�n���ɌĂяo��
	void __stdcall OnBufferStart(void* pBufferContext);
	// �o�b�t�@�̏����I�����ɌĂяo��
	void __stdcall OnBufferEnd(void* pBufferContext);
	// �����̏����p�X�I�����ɌĂяo��
	void __stdcall OnVoiceProcessingPassEnd(void);
	// �A�������X�g���[���Đ��I�����ɌĂяo��
	void __stdcall OnStreamEnd(void) {}
	// ���[�v�I���ʒu���B���ɌĂяo��
	void __stdcall OnLoopEnd(void* pBufferContext) {}
	// �G���[�������ɌĂяo��
	void __stdcall OnVoiceError(void* pBufferContext, long Error) {}

	// ���̏����f�[�^�擾
	inline size_t Bps(void) const;


	// �Q�ƃt�@�C����
	std::string name;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// ���[�v�t���O
	bool loop;

	// �d�˂�����
	unsigned int cnt;

	// �o�b�t�@����ւ��p�C���f�b�N�X
	unsigned int index;

	// �g�`�ǂݍ��݈ʒu
	std::vector<size_t>read;

	// ���݂̔g�`�f�[�^
	std::vector<std::vector<float>>data;
};
