#pragma once
#include "etc/Voice.h"
#include "Func/Func.h"
#include <memory>

class VoiceCallback;
class Manager;

class __declspec(uuid("C1FFFB4C-EC30-49DF-84ED-FD90BC1FC2AA"))
Okdio : public Voice<Okdio, IXAudio2SourceVoice>
{
	friend Voice<Okdio, IXAudio2SourceVoice>;
	friend VoiceCallback;
	friend Manager;
public:
	// �ǂݍ���
	long Load(const std::string& fileName);

	// �Đ�
	long Play(const bool& loop = false);

	// ��~
	long Stop(void);

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
