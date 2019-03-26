#pragma once
#include <deque>
#include <mutex>
#include <vector>
#include <thread>
#include <condition_variable>

class Okdio;

// エフェクト処理の適応
class Effector
{
public:
	// コンストラクタ
	Effector();
	Effector(const size_t& capacity, const size_t& threadNum);
	// デストラクタ
	~Effector();

	// キューの末尾に追加
	bool Push(Okdio* okdio);

private:
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// キューの先頭取り出し
	bool Pop(Okdio** okdio);

	// エフェクト処理実行
	void Execution(Okdio** okdio);

	// 非同期
	void Stream(void);


	// 排他制御
	std::mutex mtx;

	// 待機
	std::condition_variable cv;

	// キュー最大数
	size_t capacity;

	// スレッドフラグ
	bool threadFlag;

	// スレッド
	std::vector<std::thread>th;

	// キュー
	std::deque<Okdio*>queue;
};
