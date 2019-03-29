#pragma once
#include <deque>

// FPS�v���p
class FPS
{
public:
	// �f�X�g���N�^
	~FPS();

	// FPS�l�擾
	double GetFPS(void);

	// �C���X�^���X�ϐ��擾
	static FPS& Get(void);

private:
	// �R���X�g���N�^
	FPS();

	// ���݂̍����������~���P�ʂŎ擾
	double GetCurDefTime(void);

	// FPS�̍X�V
	double UpData(const double& def);

	
	// 1�b������̃N���b�N�J�E���g
	double freq;

	// �ȑO�̃N���b�N�J�E���g
	long long old;

	// ���ʕ����̍��v
	double sumTime;
	
	// �L���[
	std::deque<double>queue;
};
