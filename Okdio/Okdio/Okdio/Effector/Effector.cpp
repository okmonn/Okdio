#include "Effector.h"
#include "../Okdio.h"

// コンストラクタ
Effector::Effector() : 
	threadFlag(true)
{
	th = std::thread(&Effector::Stream, this);
}

// デストラクタ
Effector::~Effector()
{
	if (th.joinable() == true)
	{
		threadFlag = false;
		th.join();
	}
}

// 実行キューに追加
void Effector::Execution(Okdio* okdio)
{
	queue.push_back(okdio);
}

// 非同期
void Effector::Stream(void)
{
	while (threadFlag)
	{
		if (queue.empty())
		{
			continue;
		}

		for (auto itr = queue.begin(); itr != queue.end();)
		{
			/*for (float& i : (*itr)->wave[(*itr)->index])
			{
				i *= 2.0f;
			}

			SetEvent((*itr)->handle);
			itr = queue.erase(itr);*/
		}
	}
}

// インスタンス変数取得
Effector & Effector::Get(void)
{
	static Effector instance;
	return instance; 
}
