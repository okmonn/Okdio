#include "Effector.h"
#include "../Okdio.h"
#include "../Filter/Filter.h"
#include <algorithm>

// コンストラクタ
Effector::Effector(const unsigned int& threadNum, const unsigned int& capacity) : 
	capacity(capacity), threadFlag(true)
{
	Init();
	SetFunc();

	th.resize(threadNum);
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
		if (i.joinable() == true)
		{
			i.join();
		}
	}
}

// 初期化
void Effector::Init(void)
{
	filter = std::make_unique<Filter>();
}

// ローパスフィルタ実行
void Effector::ExecutionLowPass(Okdio* okdio)
{
	filter->LowPass(okdio->info.sample, okdio->filter.cutoff, okdio->filter.bw);
	filter->Execution(okdio->Wave(), okdio->inout[0], okdio->inout[1]);
}

// ボリューム実行
void Effector::ExecutionVolume(Okdio* okdio)
{
	for (float& i : okdio->Wave())
	{
		i *= okdio->volume;
	}
}

// 関数ポインタセット
void Effector::SetFunc(void)
{
	func[snd::Effect::LowPass] = std::bind(&Effector::ExecutionLowPass, this, std::placeholders::_1);
	func[snd::Effect::Volume]  = std::bind(&Effector::ExecutionVolume, this, std::placeholders::_1);
}

// キューに追加
bool Effector::AddQueue(Okdio* okdio)
{
	std::unique_lock<std::mutex>lock(mtx);
	if (queue.size() >= capacity)
	{
		return false;
	}

	queue.push_back(okdio);
	cv.notify_all();

	return true;
}

// 非同期
void Effector::Stream(void)
{
	while (threadFlag)
	{
		std::unique_lock<std::mutex>lock(mtx);
		while (queue.empty())
		{
			cv.wait(lock);
		}

		for (snd::Effect& type : queue.front()->type)
		{
			func[type](queue.front());
		}

		SetEvent(queue.front()->handle);
		queue.pop_front();
	}
}
