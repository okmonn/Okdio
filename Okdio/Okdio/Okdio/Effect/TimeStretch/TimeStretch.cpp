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
	//ブロックサイズ
	const unsigned int block = 40;
	//相関範囲最小値
	const unsigned int min = 1;
	//相関範囲最大値
	const unsigned int max = 10;

	//元波形のオフセット
	unsigned int offset1 = 0;
	//適応波形のオフセット
	unsigned int offset2 = 0;

	auto data = okdio->GetData();

	//適応データ
	std::vector<float>adap(data.size() / speed);

	while (offset1 < data.size())
	{
		float max_r = 0.0f;
		unsigned int offset = 0;
		for (unsigned int m = min; m <= max; ++m)
		{
			float tmp = 0.0f;
			for (unsigned int i = 0; i < block; ++i)
			{
				if (m + offset1 + i >= data.size())
				{
					break;
				}

				tmp += data[offset1 + i] * data[m + offset1 + i];
			}

			if (max_r < tmp)
			{
				tmp = max_r;
				offset = m;
			}
		}

		int q = offset / (speed - 1.0f) + 0.5f;
		for (unsigned int i = offset; i < q; ++i)
		{
			if (offset1 + i + offset > data.size())
			{
				break;
			}
			adap[offset2 + i] = data[offset1 + i + offset];
		}

		offset1 += offset + q;
		offset2 += offset;
	}

	okdio->GetData() = adap;
}

// 代入演算子オーバーロード
void TimeStretch::operator=(const TimeStretch& time)
{
	speed = time.speed;
}

