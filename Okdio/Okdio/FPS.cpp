#include "FPS.h"
#include <Windows.h>

// �L���[�ő吔
#define MAX 10

// �R���X�g���N�^
FPS::FPS() : 
	freq(0.0), old(0), sumTime(0.0)
{
	LARGE_INTEGER large{};
	if (QueryPerformanceCounter(&large) != 0)
	{
		old = large.QuadPart;
		QueryPerformanceFrequency(&large);
		freq = double(large.QuadPart);
	}

	GetFPS();
}

// �f�X�g���N�^
FPS::~FPS()
{
}

// ���݂̍����������~���P�ʂŎ擾
double FPS::GetCurDefTime(void)
{
	LARGE_INTEGER large{};
	QueryPerformanceCounter(&large);
	
	long long tmp = old;

	old = large.QuadPart;

	return double(large.QuadPart - tmp) * 1000.0 / freq;
}

// FPS�̍X�V
double FPS::UpData(const double& def)
{
	//�Â��f�[�^�폜
	if (queue.size() > 0)
	{
		queue.pop_front();
	}

	//�V�����f�[�^�ǉ�
	queue.emplace_back(def);

	//FPS�Z�o
	double fps = 0.0;
	double tmp = (sumTime + def) / MAX;
	if (tmp != 0.0)
	{
		fps = 1000.0 / tmp;
	}

	//���ʉ��Z�����̍X�V
	sumTime += def - (*queue.begin());

	return fps;
}

// FPS�l�擾
double FPS::GetFPS(void)
{
	double tmp = GetCurDefTime();
	if (tmp == 0.0)
	{
		return 0.0;
	}

	return UpData(tmp);
}

// �C���X�^���X�ϐ��擾
FPS& FPS::Get(void)
{
	static FPS instance;
	return instance;
}
