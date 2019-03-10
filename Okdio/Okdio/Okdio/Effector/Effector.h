#pragma once
#include <list>
#include <vector>
#include <thread>

class Okdio;

// エフェクター
class Effector
{
public:
	// デストラクタ
	~Effector();

	// 実行キューに追加
	void Execution(Okdio* data);

	// インスタンス変数取得
	static Effector& Get(void);

private:
	// コンストラクタ
	Effector();
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// 非同期
	void Stream(void);

	
	// スレッドフラグ
	bool threadFlag;

	// スレッド
	std::thread th;

	// キュー
	std::list<Okdio*>queue;
};
