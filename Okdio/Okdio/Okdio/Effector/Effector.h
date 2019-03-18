#pragma once
#include "Queue.h"
#include "../SoundInfo.h"
#include <mutex>
#include <vector>
#include <memory>
#include <thread>
#include <functional>
#include <unordered_map>
#include <condition_variable>

class Okdio;
class Filter;
class Comp;

// サウンドエフェクター
class Effector
{
public:
	// コンストラクタ
	Effector(const unsigned int& threadNum, const unsigned int& queueCapacity);
	// デストラクタ
	~Effector();

	// キューに追加
	bool Add(Okdio* okdio);

private:
	Effector(const Effector&) = delete;
	void operator=(const Effector&) = delete;

	// 初期化
	void Init(void);

	// ローパスフィルタ実行
	void LowPass(Okdio* okdio);

	// ハイパスフィルタ実行
	void HighPass(Okdio* okdio);

	// バンドパス実行
	void BandPass(Okdio* okdio);

	// リミッター実行
	void Limiter(Okdio* okdio);

	// コンプレッサー実行
	void Compressor(Okdio* okdio);

	// ボリューム実行
	void Volume(Okdio* okdio);

	// 関数ポインタセット
	void SetFunc(void);

	// 並列処理
	void Stream(void);


	// フィルター
	std::unique_ptr<Filter>filter;

	// 圧縮系
	std::unique_ptr<Comp>comp;

	// スレッドフラグ
	bool threadFlag;

	// キュー
	Queue<Okdio*>queue;

	// 排他制御
	std::mutex mtx;

	// 待機
	std::condition_variable cv;

	// スレッド
	std::vector<std::thread>th;

	// 関数ポインタ
	std::unordered_map<snd::Effect, std::function<void(Okdio*)>>func;
};
