#include "FPS.h"
#include <Windows.h>

// キュー最大数
#define MAX 10

// コンストラクタ
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

// デストラクタ
FPS::~FPS()
{
}

// 現在の差分時刻をミリ単位で取得
double FPS::GetCurDefTime(void)
{
	LARGE_INTEGER large{};
	QueryPerformanceCounter(&large);
	
	long long tmp = old;

	old = large.QuadPart;

	return double(large.QuadPart - tmp) * 1000.0 / freq;
}

// FPSの更新
double FPS::UpData(const double& def)
{
	//古いデータ削除
	if (queue.size() > 0)
	{
		queue.pop_front();
	}

	//新しいデータ追加
	queue.emplace_back(def);

	//FPS算出
	double fps = 0.0;
	double tmp = (sumTime + def) / MAX;
	if (tmp != 0.0)
	{
		fps = 1000.0 / tmp;
	}

	//共通加算部分の更新
	sumTime += def - (*queue.begin());

	return fps;
}

// FPS値取得
double FPS::GetFPS(void)
{
	double tmp = GetCurDefTime();
	if (tmp == 0.0)
	{
		return 0.0;
	}

	return UpData(tmp);
}

// インスタンス変数取得
FPS& FPS::Get(void)
{
	static FPS instance;
	return instance;
}
