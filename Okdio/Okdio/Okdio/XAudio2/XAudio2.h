#pragma once
#include "../etc/Singleton.h"

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2 : 
	public Singleton<XAudio2>
{
	friend Singleton<XAudio2>;
public:
	// �J�n
	bool EnginStart(void);

	// �I��
	void EnginEnd(void);

	// �I�[�f�B�I�擾
	IXAudio2* Audio(void) const;

private:
	// �R���X�g���N�^
	XAudio2();
	// �f�X�g���N�^
	~XAudio2();


	// �I�[�f�B�I����
	long CreateAudio(void);

	// �}�X�^�����O�{�C�X����
	long CreateMastering(void);


	// �I�[�f�B�I
	IXAudio2* audio;

	// �}�X�^�����O�{�C�X
	IXAudio2MasteringVoice* mastering;
};
