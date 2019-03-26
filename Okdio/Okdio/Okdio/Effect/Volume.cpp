#include "Volume.h"

// コンストラクタ
Volume::Volume()
{
	Init();
}

// コンストラクタ
Volume::Volume(const float& level)
{
	Init();
	SetLevel(level);
}

// コピーコンストラクタ
Volume::Volume(const Volume& vol)
{
	(*this) = vol;
}

// デストラクタ
Volume::~Volume()
{
}

// 初期化
void Volume::Init(void)
{
	level = 1.0f;
}

// 音量レベルセット
bool Volume::SetLevel(const float& level)
{
	if (level < 0.0f)
	{
		return false;
	}

	this->level = level;
	
	return false;
}

// 実行
void Volume::Execution(std::vector<float>& data)
{
	for (float& i : data)
	{
		i *= level;
	}
}

// 代入演算子オーバーロード
void Volume::operator=(const Volume& vol)
{
	level = vol.level;
}
