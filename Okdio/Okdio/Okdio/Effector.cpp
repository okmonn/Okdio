#include "Effector.h"
#include "Okdio.h"
#include "Effect/Effect.h"
#include <windows.h>

// コンストラクタ
Effector::Effector(const size_t& capacity, const size_t& threadNum) : 
	capacity(capacity), threadFlag(true)
{
	if (CheckThreadNum(threadNum))
	{
		th.resize(threadFlag);
		for (std::thread& i : th)
		{
			i = std::thread(&Effector::Stream, this);
		}
	}
}

// デストラクタ
Effector::~Effector()
{
	threadFlag = false;
	cv.notify_all();
	for (std::thread& i : th)
	{
		i.join();
	}
}

// キューの末尾に追加
bool Effector::Push(Okdio* okdio)
{
	std::unique_lock<std::mutex>lock(mtx);

	if (queue.size() >= capacity)
	{
		return false;
	}

	queue.emplace_back(okdio);
	cv.notify_all();

	return true;
}

// スレッド立てれるかチェック
bool Effector::CheckThreadNum(const size_t& threadNum)
{
	if (unsigned int(threadNum) < std::thread::hardware_concurrency())
	{
		return true;
	}

	return false;
}

// キューの先頭取り出し
bool Effector::Pop(Okdio** okdio)
{
	std::unique_lock<std::mutex>lock(mtx);
	while (queue.empty())
	{
		if (!threadFlag)
		{
			return false;
		}
		cv.wait(lock);
	}

	*okdio = queue.front();
	queue.pop_front();

	return true;
}

// エフェクト処理実行
void Effector::Execution(Okdio** okdio)
{
	for (Effect* i : (*okdio)->effect)
	{
		if ((*okdio)->effect.size() <= 0)
		{
			break;
		}

		i->Execution((*okdio));
	}

	std::lock_guard<std::mutex>lock(mtx);
	(*okdio)->ready = true;

	(*okdio)->cv.notify_all();
}

// 非同期
void Effector::Stream(void)
{
	while (threadFlag)
	{
		Okdio* okdio = nullptr;

		if (!Pop(&okdio))
		{
			break;
		}

		Execution(&okdio);
	}
}
