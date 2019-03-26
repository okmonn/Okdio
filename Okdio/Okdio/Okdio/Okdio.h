#pragma once
#include "etc/Info.h"
#include "Effector/Effector.h"
#include <string>
#include <vector>
#include <memory>
#include <xaudio2.h>

// �T�E���h���C�u����
class Okdio :
	IXAudio2VoiceCallback
{
	friend Effector;
public:
	// �R���X�g���N�^
	Okdio();
	Okdio(Effector* effe);
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
	long Play(const bool& loop = false, const size_t& overlaidMax = 10);
	long Play(const size_t& overlaidMax = 10, const bool& loop = false);

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

	// ���݂̔g�`���擾
	std::vector<float>& Data(void);


	// �G�t�F�N�^�[
	Effector* effe;

	// �Q�ƃt�@�C����
	std::string name;

	// �C�x���g�n���h��
	void* handle;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// ���[�v�t���O
	bool loop;

	// �d�˂�����
	size_t cnt;

	// �o�b�t�@����ւ��p�C���f�b�N�X
	unsigned int index;

	// �g�`�ǂݍ��݈ʒu
	std::vector<size_t>read;

	// ���݂̔g�`�f�[�^
	std::vector<std::vector<float>>data;
};
