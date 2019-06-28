#pragma once
#include "../etc/Singleton.h"

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2 : 
	public Singleton<XAudio2>
{
	friend Singleton<XAudio2>;
public:
	// �I�[�f�B�I�擾
	IXAudio2* Audio(void) const;

	// �}�X�^�����O�{�C�X�擾
	IXAudio2MasteringVoice* Mastering(void) const;

private:
	// �R���X�g���N�^
	XAudio2();
	// �f�X�g���N�^
	~XAudio2();

	// COM������
	void InitCom(void);

	// �I�[�f�B�I����
	void CreateAudio(void);

	// �}�X�^�����O�{�C�X�̐���
	void CreateMastering(void);


	// �I�[�f�B�I
	IXAudio2* audio;

	// �}�X�^�����O�{�C�X
	IXAudio2MasteringVoice* mastering;
};
