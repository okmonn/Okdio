#include "TimeStretch.h"
#include "../../Okdio.h"

// コンストラクタ
TimeStretch::TimeStretch()
{
	Init();
}

// コンストラクタ
TimeStretch::TimeStretch(const float& speed)
{
	Init();
	SetSpeed(speed);
}

// コピーコンストラクタ
TimeStretch::TimeStretch(const TimeStretch& time)
{
	(*this) = time;
}

// デストラクタ
TimeStretch::~TimeStretch()
{
}

// 再生速度セット
bool TimeStretch::SetSpeed(const float& speed)
{
	if (speed <= 0.0f)
	{
		return false;
	}

	this->speed = speed;

	return true;
}

// 初期化
void TimeStretch::Init(void)
{
	speed = 1.0f;
}

// 実行
void TimeStretch::Execution(Okdio* okdio)
{
	auto data = okdio->GetData();

	int m = 0;
}

// 代入演算子オーバーロード
void TimeStretch::operator=(const TimeStretch& time)
{
	speed = time.speed;
}

