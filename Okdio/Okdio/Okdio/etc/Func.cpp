#include "Func.h"
#include "../Loader/Loader.h"

// サウンドデータ削除
void snd::Delete(const std::string& fileName)
{
	Loader::Get().Delete(fileName);
}

// 任意の桁から切り捨て
float snd::Floor(const float& input, const int& digit)
{
	float tmp = input;
	tmp *= std::pow(10.0f, -digit - 1);
	tmp  = std::floor(tmp);
	tmp *= std::pow(10.0f, digit + 1);

	return tmp;
}

// 円周率取得
constexpr float snd::PI(void)
{
	return 3.14159265358979323846f;
}

// 離散フーリエ変換
void snd::DFT(const std::vector<float>& data, std::vector<std::complex<float>>& arg)
{
	size_t size = data.size();
	arg.assign(size, 0.0f);

	for (size_t i = 0; i < size; ++i)
	{
		float real = 0.0f;
		float imag = 0.0f;
		for (size_t n = 0; n < size; ++n)
		{
			real +=  data[n] * std::cos(2.0f * snd::PI() * i * n / size);
			imag += -data[n] * std::sin(2.0f * snd::PI() * i * n / size);
		}

		arg[i] = { real, imag };
	}
}

// 離散フーリエ変換
void snd::DFT(const std::vector<double>& data, std::vector<std::complex<double>>& arg)
{
	size_t size = data.size();
	arg.assign(size, 0.0);

	for (size_t i = 0; i < size; ++i)
	{
		double real = 0.0;
		double imag = 0.0;
		for (size_t n = 0; n < size; ++n)
		{
			real +=  data[n] * std::cos(2.0 * double(snd::PI()) * i * n / size);
			imag += -data[n] * std::sin(2.0 * double(snd::PI()) * i * n / size);
		}

		arg[i] = { real, imag };
	}
}

// 逆離散フーリエ変換
std::vector<float> snd::IDFT(const std::vector<std::complex<float>>& arg)
{
	size_t size = arg.size();
	std::vector<float>data(size, 0.0f);

	for (size_t i = 0; i < size; ++i)
	{
		for (size_t n = 0; n < size; ++n)
		{
			data[i] += arg[n].real() * std::cos(2.0f * snd::PI() * i * n / size)
				     - arg[n].imag() * std::sin(2.0f * snd::PI() * i * n / size);
		}

		data[i] /= size;
	}

	return data;
}

// 逆離散フーリエ変換
std::vector<double> snd::IDFT(const std::vector<std::complex<double>>& arg)
{
	size_t size = arg.size();
	std::vector<double>data(size, 0.0f);

	for (size_t i = 0; i < size; ++i)
	{
		for (size_t n = 0; n < size; ++n)
		{
			data[i] += arg[n].real() * std::cos(2.0 * double(snd::PI()) * i * n / size)
				     - arg[n].imag() * std::sin(2.0 * double(snd::PI()) * i * n / size);
		}

		data[i] /= size;
	}

	return data;
}
