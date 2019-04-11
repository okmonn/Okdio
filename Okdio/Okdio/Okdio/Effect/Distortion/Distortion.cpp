#include "Distortion.h"
#include "../../OKdio.h"
#include <algorithm>

// コンストラクタ
Distortion::Distortion()
{
	Init();
}

// コンストラクタ
Distortion::Distortion(const float& amp)
{
	Init();
	SetAmp(amp);
}

// コピーコンストラクタ
Distortion::Distortion(const Distortion& dis)
{
	(*this) = dis;
}

// デストラクタ
Distortion::~Distortion()
{
}

// 初期化
void Distortion::Init(void)
{
	amp = 1.0f;
}

// 増幅率セット
bool Distortion::SetAmp(const float& amp)
{
	if (amp < 1.0f)
	{
		return false;
	}

	this->amp = amp;

	return true;
}

// 実行
void Distortion::Execution(Okdio* okdio)
{
	if (amp <= 1.0f)
	{
		return;
	}

	for (float& i : okdio->GetData())
	{
		i *= amp;
		i  = std::min(i,  1.0f);
		i  = std::max(i, -1.0f);
	}
}

// 代入演算子オーバーロード
void Distortion::operator=(const Distortion& dis)
{
	amp = dis.amp;
}
