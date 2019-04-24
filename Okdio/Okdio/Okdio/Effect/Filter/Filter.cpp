#include "Filter.h"
#include "../../OKdio.h"
#include "../../etc/Func.h"

// カットオフ周波数最小値
#define CUT_MIN 10.0f

// コンストラクタ
Filter::Filter()
{
	Init();
	InitFunc();
}

// コンストラクタ
Filter::Filter(const snd::FilterType& type, const float& cutoff, const float& q, const unsigned short& sample)
{
	Init();
	InitFunc();

	if (func1.find(type) != func1.end())
	{
		func1[type](cutoff, q, sample);
	}
}

// コピーコンストラクタ
Filter::Filter(const Filter& filter)
{
	(*this) = filter;
}

// デストラクタ
Filter::~Filter()
{
}

// 初期化
void Filter::Init(void)
{
	memset(&a[0], 0, sizeof(a));
	memset(&b[0], 0, sizeof(b));

	a[0] = b[0] = 1.0f;
}

// 関数ポインタ初期化
void Filter::InitFunc(void)
{
	func1[snd::FilterType::LowPass]  = std::bind(&Filter::LowPass,  this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	func1[snd::FilterType::HighPass] = std::bind(&Filter::HighPass, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	func1[snd::FilterType::BandPass] = std::bind(&Filter::BandPass, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	func1[snd::FilterType::AllPass]  = std::bind(&Filter::AllPass,  this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	func1[snd::FilterType::Notch]    = std::bind(&Filter::Notch,    this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

	func2[snd::FilterType::LowShelf]  = std::bind(&Filter::LowShelf,  this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	func2[snd::FilterType::HighShelf] = std::bind(&Filter::HighShelf, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
	func2[snd::FilterType::Peaking]   = std::bind(&Filter::Peaking,   this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
}

// ローパスフィルタ
bool Filter::LowPass(const float& cutoff, const float& q, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (q <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / 2.0f * q;

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] = (1.0f - std::cos(omega)) / 2.0f;
	b[1] =  1.0f - std::cos(omega);
	b[2] = (1.0f - std::cos(omega)) / 2.0f;

	return true;
}

// ハイパスフィルタ
bool Filter::HighPass(const float& cutoff, const float& q, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (q <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / 2.0f * q;

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  (1.0f + std::cos(omega)) / 2.0f;
	b[1] = -(1.0f + std::cos(omega));
	b[2] =  (1.0f + std::cos(omega)) / 2.0f;

	return true;
}

// バンドパスフィルタ
bool Filter::BandPass(const float& cutoff, const float& bw, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (bw <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) * std::sinh(std::log(2.0f) / 2.0f * bw * omega / std::sin(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  alpha;
	b[1] =  0.0f;
	b[2] = -alpha;

	return true;
}

// オールパスフィルタ
bool Filter::AllPass(const float& cutoff, const float& q, const unsigned int& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (q <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / 2.0f * q;

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  1.0f - alpha;
	b[1] = -2.0f * cos(omega);
	b[2] =  1.0f + alpha;

	return true;
}

// ノッチフィルタ
bool Filter::Notch(const float& cutoff, const float& bw, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (bw <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) * std::sinh(std::log(2.0f) / 2.0f * bw * omega / std::sin(omega));

	a[0] =  1.0f + alpha;
	a[1] = -2.0f * std::cos(omega);
	a[2] =  1.0f - alpha;

	b[0] =  1.0f;
	b[1] = -2.0f * std::cos(omega);
	b[2] =  1.0f;

	return true;
}

// ローシェルフフィルタ
bool Filter::LowShelf(const float& cutoff, const float& q, const float& gain, const unsigned short& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (q <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / 2.0f * q;
	float A     = std::pow(10.0f, gain / 40.0f);
	float beta  = std::sqrt(A) / q;

	a[0] = (A + 1.0f) + (A - 1.0f) * std::cos(omega) + beta * std::sin(omega);
	a[1] = -2.0f * ((A - 1.0f) + (A + 1.0f) * std::cos(omega));
	a[2] = (A + 1.0f) + (A - 1.0f) * std::cos(omega) - beta * std::sin(omega);

	b[0] = A * ((A + 1.0f) - (A - 1.0f) * std::cos(omega) + beta * std::sin(omega));
	b[1] = 2.0f * A * ((A - 1.0f) - (A + 1.0f) * std::cos(omega));
	b[2] = A * ((A + 1.0f) - (A - 1.0f) * std::cos(omega) - beta * std::sin(omega));

	return true;
}

// ハイシェルフフィルタ
bool Filter::HighShelf(const float& cutoff, const float& q, const float& gain, const unsigned int& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (q <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) / 2.0f * q;
	float A     = std::pow(10.0f, gain / 4.0f);
	float beta  = std::sqrt(A) / q;

	a[0] = (A + 1.0f) - (A - 1.0f) * cos(omega) + beta * sin(omega);
	a[1] =  2.0f * ((A - 1.0f) - (A + 1.0f) * cos(omega));
	a[2] = (A + 1.0f) - (A - 1.0f) * cos(omega) - beta * sin(omega);

	b[0] =  A * ((A + 1.0f) + (A - 1.0f) * cos(omega) + beta * sin(omega));
	b[1] = -2.0f * A * ((A - 1.0f) + (A + 1.0f) * cos(omega));
	b[2] =  A * ((A + 1.0f) + (A - 1.0f) * cos(omega) - beta * sin(omega));

	return true;
}

// ピーキングフィルタ
bool Filter::Peaking(const float& cutoff, const float& bw, const float& gain, const unsigned int& sample)
{
	if (cutoff < CUT_MIN
		|| cutoff > snd::Floor(float(sample / 2), 3))
	{
		return false;
	}

	if (bw <= 0.0f)
	{
		return false;
	}

	float omega = 2.0f * snd::PI() * cutoff / sample;
	float alpha = std::sin(omega) * std::sinh(std::log(2.0f) / 2.0f * bw * omega / std::sin(omega));
	float A     = std::pow(10.0f, gain / 40.0f);

	a[0] =  1.0f + alpha / A;
	a[1] = -2.0f * cos(omega);
	a[2] =  1.0f - alpha / A;

	b[0] =  1.0f + alpha * A;
	b[1] = -2.0f * cos(omega);
	b[2] =  1.0f - alpha * A;

	return true;
}

// 実行
void Filter::Execution(Okdio* okdio)
{
	float* inout = okdio->GetInOut();
	for (float& i : okdio->GetData())
	{
		float tmp = i;

		i = b[0] / a[0] * tmp
		  + b[1] / a[0] * inout[0]
		  + b[2] / a[0] * inout[1]
		  - a[1] / a[0] * inout[2]
		  - a[2] / a[0] * inout[3];

		//入力値の更新
		inout[1] = inout[0];
		inout[0] = tmp;

		//出力値の更新
		inout[3] = inout[2];
		inout[2] = i;
	}
}

// 代入演算子オーバーロード
void Filter::operator=(const Filter& filter)
{
	memcpy(a, &filter.a[0], sizeof(a));
	memcpy(b, &filter.b[0], sizeof(b));
}
