#pragma once
#include "../SoundInfo.h"
#include <deque>
#include <mutex>
#include <vector>
#include <thread>
#include <functional>
#include <unordered_map>
#include <condition_variable>

class Okdio;
class Filter;

// サウンドエフェクト中心クラス
class Effector
{
public:
	// コンストラクタ
	Effector(const unsigned int& threadNum, const unsigned int& capacity);
	// デストラクタ
	~Effector();

	// キューに追加
	bool AddQueue(Okdio* okdio);

private:
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// 初期化
	void Init(void);

	// ローパスフィルタ実行
	void ExecutionLowPass(Okdio* okdio);

	// ボリューム実行
	void ExecutionVolume(Okdio* okdio);

	// 関数ポインタセット
	void SetFunc(void);

	// 非同期
	void Stream(void);


	// デジタルフィルタ
	std::unique_ptr<Filter>filter;

	// 排他制御
	std::mutex mtx;

	// 待機
	std::condition_variable cv;

	// キュー最大数
	unsigned int capacity;

	// スレッドフラグ
	bool threadFlag;

	// キュー
	std::deque<Okdio*>queue;

	// スレッド
	std::vector<std::thread>th;

	// 関数ポインタ
	std::unordered_map<snd::Effect, std::function<void(Okdio*)>>func;
};
