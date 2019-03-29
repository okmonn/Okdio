#include "Pan.h"
#include "../../OKdio.h"

#define RAD(X) (X) * snd::PI() / 180.0f

// コンストラクタ
Pan::Pan()
{
	Init();
}

// コンストラクタ
Pan::Pan(const float& angle)
{
	Init();
	SetPanning(angle);
}

// コピーコンストラクタ
Pan::Pan(const Pan& pan)
{
	(*this) = pan;
}

// デストラクタ
Pan::~Pan()
{
}

// 初期化
void Pan::Init(void)
{
	angle = 0.0f;
}

// 定位角度セット
bool Pan::SetPanning(const float& angle)
{
	if (angle < -180.0f
		|| angle > 180.0f)
	{
		return false;
	}

	this->angle = angle;

	return true;
}

// 実行
void Pan::Execution(Okdio* okdio)
{
	if (okdio->GetInfo().channel <= 1 || angle == 0.0f)
	{
		return;
	}

	std::vector<float>& data = okdio->GetData();
	size_t ch = (angle > 0.0f) ? 0 : 1;
	for (size_t i = ch; i < data.size(); i += okdio->GetInfo().channel)
	{
		data[i] *= std::cos(RAD(angle * angle / 90.0f));
	}
}

// 代入演算子オーバーロード
void Pan::operator=(const Pan& pan)
{
	angle = pan.angle;
}
