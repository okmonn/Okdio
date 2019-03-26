#include "Effector.h"
#include "../OKdio.h"

// コンストラクタ
Effector::Effector()
{
}

// コンストラクタ
Effector::Effector(const size_t& capacity, const size_t& threadNum) : 
	capacity(capacity), threadFlag(true)
{
	th.resize(threadFlag);
	for (std::thread& i : th)
	{
		i = std::thread(&Effector::Stream, this);
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
	std::mutex mtx;
	std::unique_lock<std::mutex>lock(mtx);

	for (float& i : (*okdio)->Data())
	{
		i *= 10.0f;
	}

	SetEvent((*okdio)->handle);
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
