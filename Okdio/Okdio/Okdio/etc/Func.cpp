#include "Func.h"

// ”CˆÓ‚ÌŒ…‚©‚çØ‚èÌ‚Ä
float snd::Floor(const float& input, const int& digit)
{
	float tmp = input;
	tmp *= std::pow(10.0f, -digit - 1);
	tmp  = std::floor(tmp);
	tmp *= std::pow(10.0f, digit + 1);

	return tmp;
}

// ‰~ü—¦æ“¾
constexpr float snd::PI(void)
{
	return 3.14159265358979323846f;
}

// —£Uƒt[ƒŠƒG•ÏŠ·
void snd::DFT(const std::vector<float>& data, std::vector<float>& real, std::vector<float>& imag)
{
	size_t size = data.size();
	real.assign(size, 0.0f);
	imag.assign(size, 0.0f);

	for (size_t i = 0; i < size; ++i)
	{
		for (size_t n = 0; n < size; ++n)
		{
			real[i] +=  data[n] * std::cos(2.0f * snd::PI() * i * n / size);
			imag[i] += -data[n] * std::sin(2.0f * snd::PI() * i * n / size);
		}
	}
}

// ‹t—£Uƒt[ƒŠƒG•ÏŠ·
std::vector<float> snd::IDFT(const std::vector<float>& real, const std::vector<float>& imag)
{
	size_t size = real.size();
	std::vector<float>data(size, 0.0f);

	for (size_t i = 0; i < size; ++i)
	{
		for (size_t n = 0; n < size; ++n)
		{
			data[i] += real[n] * std::cos(2.0f * snd::PI() * i * n / size)
				     - imag[n] * std::sin(2.0f * snd::PI() * i * n / size);
		}

		data[i] /= size;
	}

	return data;
}
