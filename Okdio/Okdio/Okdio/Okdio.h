#pragma once
#include "etc/Info.h"
#include "etc/Func.h"
#include <string>
#include <vector>
#include <memory>

struct IXAudio2SourceVoice;
class VoiceCallback;
class Effector;
class Effect;

// �T�E���h���C�u����
class Okdio
{
	friend VoiceCallback;
	friend Effector;
public:
	// �R���X�g���N�^
	Okdio();
	Okdio(Effector* effector);
	Okdio(const std::string& fileName);
	Okdio(const snd::Info& info, const std::vector<float>& data);
	Okdio(const std::string& fileName, Effector* effector);
	Okdio(const snd::Info& info, const std::vector<float>& data, Effector* effector);
	// �R�s�[�R���X�g���N�^
	Okdio(const Okdio& okdio);
	// �f�X�g���N�^
	~Okdio();

	// �t�@�C������ǂݍ���
	int Load(const std::string& fileName);

	// �I���W�i���g�`����
	int CreateOriginal(const snd::Info & info, const std::vector<float>& data);

	// �G�t�F�N�g�𖖔��ɒǉ�
	void PushEffect(Effect* effect);

	// �G�t�F�N�g���܂Ƃ߂ăZ�b�g
	void SetEffect(const std::initializer_list<Effect*>& effect);

	// �Đ�
	// loop�@false�F���[�v���Ȃ��Atrue�F���[�v����
	// overlaidMax�@�f�B���C�̏d�ˊ|���ő�l��ݒ肵�Ă�������
	long Play(const bool& loop = false, const size_t& overlaidMax = 10);

	// ��~
	long Stop(void);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Okdio& okdio);

	// �T�E���h���擾
	snd::Info GetInfo(void) const;

	// �Đ��I���m�F
	bool IsPlayEnd(void) const;

	// �Q�ƃt�@�C���p�X�擾
	std::string GetName(void) const;

	// ���݂̔g�`���擾
	std::vector<float>& GetData(void);

	// �t�B���^�p���o�̓f�[�^�擾
	float* GetInOut(void);

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

	// ���̏����f�[�^�擾
	inline size_t Bps(void) const;


	// �{�C�X�R�[���o�b�N
	std::unique_ptr<VoiceCallback>back;

	// �G�t�F�N�^�[
	Effector* effector;

	// �Q�ƃt�@�C����
	std::string name;

	// �C�x���g�n���h��
	void* handle;

	// �\�[�X�{�C�X
	IXAudio2SourceVoice* voice;

	// �Đ��I���t���O
	bool endFlag;

	// ���[�v�t���O
	bool loop;

	// �d�˂�����
	size_t cnt;

	// �o�b�t�@����ւ��p�C���f�b�N�X
	unsigned int index;

	// �t�B���^�p���o��
	float inout[4];

	// �g�`�ǂݍ��݈ʒu
	std::vector<size_t>read;

	// ���݂̔g�`�f�[�^
	std::vector<std::vector<float>>data;

	// �G�t�F�N�g
	std::vector<Effect*>effect;
};
