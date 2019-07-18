#pragma once
#include "MixVoice/MixVoice.h"
#include "Func/Func.h"
#include <memory>

class VoiceCallback;
class Manager;

class __declspec(uuid("00000000-0917-0917-0917-000000000001"))
Okdio : public Voice<Okdio, IXAudio2SourceVoice>
{
	friend Voice<Okdio, IXAudio2SourceVoice>;
	friend VoiceCallback;
	friend Manager;
public:
	// �ǂݍ���
	long Load(const std::string& fileName);

	// �~�b�N�X�{�C�X�Z�b�g
	long SetMixVoice(MixVoice* mix, const size_t& num);

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

	// �I���t���O�擾
	bool IsPlayEnd(void) const;

private:
	// �R���X�g���N�^
	Okdio();
	Okdio(const Okdio&) = delete;
	void operator=(const Okdio&) = delete;
	// �f�X�g���N�^
	~Okdio();

	// �\�[�X�{�C�X����
	long CreateVoice(void);

	// �I�[�f�B�I�o�b�t�@�ɒǉ�
	void Submit(void);

	// �I���`�F�b�N
	void CheckEnd(void);


	// �R�[���o�b�N
	std::unique_ptr<VoiceCallback>callback;

	// �Q�ƃt�@�C����
	std::string name;

	// �ǂݍ��݈ʒu
	size_t read;

	// ���[�v�t���O
	bool loop;

	// �I���t���O
	bool end;
};
