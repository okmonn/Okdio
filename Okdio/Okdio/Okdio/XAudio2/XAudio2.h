#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

// XAudio2
class XAudio2
{
public:
	// �f�X�g���N�^
	~XAudio2();

	// �C���X�^���X�ϐ��擾
	static XAudio2& Get(void);
	// �I�[�f�B�I�擾
	IXAudio2* Audio(void) const;
	// �}�X�^�����O�擾
	IXAudio2MasteringVoice* Mastering(void) const;

private:
	// �R���X�g���N�^
	XAudio2();
	XAudio2(const XAudio2&) = delete;
	void operator=(const XAudio2&) = delete;

	// COM������
	long InitCom(void) const;

	// �I�[�f�B�I����
	long CreateAudio(void);

	// �}�X�^�����O����
	long CreateMastering(void);

	// ������
	void Init(void);


	// �I�[�f�B�I
	IXAudio2* audio;

	// �}�X�^�����O
	IXAudio2MasteringVoice* mastering;
};
