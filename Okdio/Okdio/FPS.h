#pragma once
#include <deque>

// FPS計測用
class FPS
{
public:
	// デストラクタ
	~FPS();

	// FPS値取得
	double GetFPS(void);

	// インスタンス変数取得
	static FPS& Get(void);

private:
	// コンストラクタ
	FPS();

	// 現在の差分時刻をミリ単位で取得
	double GetCurDefTime(void);

	// FPSの更新
	double UpData(const double& def);

	
	// 1秒当たりのクロックカウント
	double freq;

	// 以前のクロックカウント
	long long old;

	// 共通部分の合計
	double sumTime;
	
	// キュー
	std::deque<double>queue;
};
