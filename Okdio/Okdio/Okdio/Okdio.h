#pragma once
#include "SoundInfo.h"
#include "Effector/Effector.h"
#include <string>
#include <vector>
#include <optional>
#include <xaudio2.h>
#include <memory>

class Okdio :
	public IXAudio2VoiceCallback
{
	friend Effector;
public:
	// �R���X�g���N�^
	Okdio();
	Okdio(std::weak_ptr<Effector>effe);
	Okdio(const std::string& fileName);
	Okdio(const snd::Info& info, const std::vector<float>& data);
	// �R�s�[�R���X�g���N�^
	Okdio(const Okdio& okdio);
	// �f�X�g���N�^
	~Okdio();

	// �ǂݍ���
	int Load(const std::string& fileName);

	// �I���W�i�����Z�b�g
	int SetInfo(const snd::Info& info, const std::vector<float>& data);

	// �f�W�^���t�B���^�p�����[�^�Z�b�g
	bool SetFilterParam(const float& cutoff, const float& bw = 1.0f / std::sqrt(2.0f));

	// �{�����[���Z�b�g
	bool SetVolume(const float& volume);

	// �G�t�F�N�g�Z�b�g
	void SetEffect(const std::initializer_list<snd::Effect>& type);

	// �G�t�F�N�g�ǉ�
	void AddEffect(const snd::Effect& type);

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

	// ���
	void operator=(const Okdio& okdio);

private:
	// ������
	void Init(void);

	// �G�t�F�N�g�p�����[�^������
	void EffeInit(void);

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

	// ���݂̔g�`�f�[�^
	std::vector<float>& Wave(void);

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


	// �G�t�F�N�^�[
	std::weak_ptr<Effector>effe;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �Q�ƃt�@�C����
	std::optional<std::string>name;

	// �o�b�t�@����ւ��p�C���f�b�N�X
	unsigned int index;

	// �d�˂�����
	unsigned int cnt;

	// ���[�v�t���O
	bool loop;

	// �G�t�F�N�g���������n���h��
	void* handle;

	// �T�E���h���
	snd::Info info;

	// �I���W�i���g�`�f�[�^
	std::vector<float>original;

	// �ǂݍ��݈ʒu
	std::vector<unsigned int>read;

	// �g�`�f�[�^
	std::vector<std::vector<float>>wave;

	// �G�t�F�N�g�K���ꗗ
	std::vector<snd::Effect>type;

	// �f�W�^���t�B���^�p���o�͒l
	float inout[2][2];

	// �f�W�^���t�B���^�p�����[�^
	snd::FilterParam filter;

	// �{�����[��
	float volume;
};
