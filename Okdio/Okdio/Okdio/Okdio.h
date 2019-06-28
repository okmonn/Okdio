#pragma once
#include "MixVoice/MixVoice.h"
#include "etc/SoundEffects.h"
#include "Microphone/Microphone.h"
#include <string>
#include <memory>

class VoiceCallback;

class Okdio : 
	public Voice<IXAudio2SourceVoice>
{
	friend VoiceCallback;
public:
	// �R���X�g���N�^
	Okdio();
	Okdio(const std::string& fileName, MixVoice* mix = nullptr, const size_t& num = 0);
	Okdio(const snd::Info& info, const std::vector<float>& data);
	// �f�X�g���N�^
	~Okdio();

	// �ǂݍ���
	void Load(const std::string& fileName, MixVoice* mix = nullptr, const size_t& num = 0);

	// �J�X�^���g�`�o�^
	void CustomWave(const snd::Info& info, const std::vector<float>& data);

	// �Đ�
	void Play(const bool& loop = false);

	// ��~
	void Stop(void);

	// �Đ����x����
	bool SetSpeed(const float& rate);

	// �o�^���擾
	std::string Name(void) const;

	// �ǂݍ��݈ʒu�擾
	size_t& Read(void);

	// ������Z�q�I�[�o�[���[�h
	void operator=(const Okdio& okdio);

private:
	// ������
	void Init(void);

	// �\�[�X�{�C�X����
	void CreateVoice(void);

	// �g�`�f�[�^���{�C�X�o�b�t�@�ɒǉ�
	void Submit(void);

	// �I���m�F
	void CheckEnd(void);

	// �~�b�N�X�{�C�X�Z�b�g
	void SetMixVoice(MixVoice* mix, const size_t& num);


	// �R�[���o�b�N
	std::unique_ptr<VoiceCallback>back;

	// �T�E���h�Q�Ɩ�
	std::string name;

	// �ǂݍ��݈ʒu
	size_t read;

	// ���[�v�t���O
	bool loop;

	// �Đ��I���t���O
	bool end;
};
