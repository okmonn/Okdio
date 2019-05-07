#include "Func.h"
#include "../Loader/Loader.h"

// �T�E���h�f�[�^�폜
void snd::Delete(const std::string& fileName)
{
	Loader::Get().Delete(fileName);
}

// �C�ӂ̌�����؂�̂�
float snd::Floor(const float& input, const int& digit)
{
	float tmp = input;
	tmp *= std::pow(10.0f, -digit - 1);
	tmp  = std::floor(tmp);
	tmp *= std::pow(10.0f, digit + 1);

	return tmp;
}

// �~�����擾
constexpr float snd::PI(void)
{
	return 3.14159265358979323846f;
}

// �n�j���O��
template <typename T>
T snd::Haninng(unsigned int& i, size_t& num)
{
	return (num % 2 == 0)
		? T(0.5 - 0.5 * std::cos(2.0 * snd::PI() * i / num))
		: T(0.5 - 0.5 * std::cos(2.0 * snd::PI() * (i + 0.5) / num));
}
template float snd::Haninng<float>(unsigned int&, size_t&);
template double snd::Haninng<double>(unsigned int&, size_t&);

// ���U�t�[���G�ϊ�
std::vector<std::complex<float>> snd::DFT(const std::vector<float>& data)
{
	size_t size = data.size();
	std::vector<std::complex<float>>arg(size);

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

	return arg;
}

// ���U�t�[���G�ϊ�
std::vector<std::complex<double>> snd::DFT(const std::vector<double>& data)
{
	size_t size = data.size();
	std::vector<std::complex<double>>arg(size);

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

	return arg;
}

// �t���U�t�[���G�ϊ�
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

// �t���U�t�[���G�ϊ�
std::vector<double> snd::IDFT(const std::vector<std::complex<double>>& arg)
{
	size_t size = arg.size();
	std::vector<double>data(size, 0.0f);

	for (size_t i = 0; i < size; ++i)
	{
		for (size_t n = 0; n < size; ++n)
		{
			data[i] += arg[n].real() * std::cos(2.0 * snd::PI() * i * n / size)
				     - arg[n].imag() * std::sin(2.0 * snd::PI() * i * n / size);
		}

		data[i] /= size;
	}

	return data;
}

// �����t�[���G�ϊ�
std::vector<std::complex<float>> snd::FFT(const std::vector<float>& data)
{
	//2�̗ݏ�ɒ���
	unsigned int stage = unsigned int(std::ceil(std::log2(data.size())));
	unsigned int num   = unsigned int(std::pow(2, stage));

	//���ёւ��p�z��
	std::vector<unsigned int>index(num, 0);

	//�K���f�[�^�z��
	std::vector<std::complex<float>>tmp(data.begin(), data.end());
	tmp.resize(num, 0.0);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < unsigned int(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < unsigned int(std::pow(2, stage - st)); ++n)
			{
				//�z��ԍ�1
				unsigned int index1 = unsigned int(std::pow(2, stage - st + 1)) * i + n;
				//�z��ԍ�2
				unsigned int index2 = unsigned int(std::pow(2, stage - st)) + index1;

				float p = std::pow(2.0f, float(st - 1)) * n;

				std::complex<float>tmp1 = tmp[index1];
				std::complex<float>tmp2 = tmp[index2];
				std::complex<float>tmp3 = { std::cos(2.0f * snd::PI() * p / num), -std::sin(2.0f * snd::PI() * p / num) };

				tmp[index1] = tmp1 + tmp2;
				if (st < stage)
				{
					tmp[index2].real((tmp1.real() - tmp2.real()) * tmp3.real() - (tmp1.imag() - tmp2.imag()) * tmp3.imag());
					tmp[index2].imag((tmp1.imag() - tmp2.imag()) * tmp3.real() + (tmp1.real() - tmp2.real()) * tmp3.imag());
				}
				else
				{
					tmp[index2] = tmp1 - tmp2;
				}
			}

			unsigned int m = unsigned int(std::pow(2, st - 1)) + i;
			index[m] = index[i] + unsigned int(std::pow(2, stage - st));
		}
	}

	//���ёւ�
	for (unsigned int i = 0; i < num; ++i)
	{
		if (index[i] > i)
		{
			std::swap(tmp[index[i]], tmp[i]);
		}
	}

	return tmp;
}

// �����t�[���G�ϊ�
std::vector<std::complex<double>> snd::FFT(const std::vector<double>& data)
{
	//2�̗ݏ�ɒ���
	unsigned int stage = unsigned int(std::ceil(std::log2(data.size())));
	unsigned int num   = unsigned int(std::pow(2, stage));

	//���ёւ��p�z��
	std::vector<unsigned int>index(num, 0);

	//�K���f�[�^�z��
	std::vector<std::complex<double>>tmp(data.begin(), data.end());
	tmp.resize(num, 0.0);

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < unsigned int(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < unsigned int(std::pow(2, stage - st)); ++n)
			{
				//�z��ԍ�1
				unsigned int index1 = unsigned int(std::pow(2, stage - st + 1)) * i + n;
				//�z��ԍ�2
				unsigned int index2 = unsigned int(std::pow(2, stage - st)) + index1;

				double p = std::pow(2, st - 1) * n;

				std::complex<double>tmp1 = tmp[index1];
				std::complex<double>tmp2 = tmp[index2];
				std::complex<double>tmp3 = { std::cos(2 * snd::PI() * p / num), -std::sin(2 * snd::PI() * p / num) };

				tmp[index1] = tmp1 + tmp2;
				if (st < stage)
				{
					tmp[index2].real((tmp1.real() - tmp2.real()) * tmp3.real() - (tmp1.imag() - tmp2.imag()) * tmp3.imag());
					tmp[index2].imag((tmp1.imag() - tmp2.imag()) * tmp3.real() + (tmp1.real() - tmp2.real()) * tmp3.imag());
				}
				else
				{
					tmp[index2] = tmp1 - tmp2;
				}
			}

			unsigned int m = unsigned int(std::pow(2, st - 1)) + i;
			index[m] = index[i] + unsigned int(std::pow(2, stage - st));
		}
	}

	//���ёւ�
	for (unsigned int i = 0; i < num; ++i)
	{
		if (index[i] > i)
		{
			std::swap(tmp[index[i]], tmp[i]);
		}
	}

	return tmp;
}

// �t�����t�[���G�ϊ�
std::vector<float> snd::IFFT(const std::vector<std::complex<float>>& data, const int& num)
{
	//2�̊�����߂�
	unsigned int stage = unsigned int(std::log2(data.size()));

	//���ёւ��p�z��
	std::vector<unsigned int>index(data.size());

	//�K���f�[�^
	std::vector<std::complex<float>>tmp = data;

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < unsigned int(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < unsigned int(std::pow(2, stage - st)); ++n)
			{
				//�z��ԍ�1
				unsigned int index1 = unsigned int(std::pow(2, stage - st + 1)) * i + n;
				//�z��ԍ�2
				unsigned int index2 = unsigned int(std::pow(2, stage - st)) + index1;

				float p = std::pow(2.0f, float(st - 1)) * n;

				std::complex<float>tmp1 = tmp[index1];
				std::complex<float>tmp2 = tmp[index2];
				std::complex<float>tmp3 = { std::cos(2.0f * snd::PI() * p / data.size()), std::sin(2.0f * snd::PI() * p / data.size()) };

				tmp[index1] = tmp1 + tmp2;
				if (st < stage)
				{
					tmp[index2].real((tmp1.real() - tmp2.real()) * tmp3.real() - (tmp1.imag() - tmp2.imag()) * tmp3.imag());
					tmp[index2].imag((tmp1.imag() - tmp2.imag()) * tmp3.real() + (tmp1.real() - tmp2.real()) * tmp3.imag());
				}
				else
				{
					tmp[index2] = tmp1 - tmp2;
				}
			}

			unsigned int m = unsigned int(std::pow(2, st - 1)) + i;
			index[m] = index[i] + unsigned int(std::pow(2, stage - st));
		}
	}

	std::vector<float>real(data.size());

	//���ёւ�
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		if (index[i] > i)
		{
			std::swap(tmp[index[i]], tmp[i]);
		}

		tmp[i] /= float(data.size());
		real[i] = tmp[i].real();
	}

	size_t n = (num < 0) ? data.size() - 1 : num;

	return std::vector<float>(&real[0], &real[n]);
}

// �t�����t�[���G�ϊ�
std::vector<double> snd::IFFT(const std::vector<std::complex<double>>& data, const int& num)
{
	//2�̊�����߂�
	unsigned int stage = unsigned int(std::log2(data.size()));

	//���ёւ��p�z��
	std::vector<unsigned int>index(data.size());

	//�K���f�[�^
	std::vector<std::complex<double>>tmp = data;

	for (unsigned int st = 1; st <= stage; ++st)
	{
		for (unsigned int i = 0; i < unsigned int(std::pow(2, st - 1)); ++i)
		{
			for (unsigned int n = 0; n < unsigned int(std::pow(2, stage - st)); ++n)
			{
				//�z��ԍ�1
				unsigned int index1 = unsigned int(std::pow(2, stage - st + 1)) * i + n;
				//�z��ԍ�2
				unsigned int index2 = unsigned int(std::pow(2, stage - st)) + index1;

				double p = std::pow(2, st - 1) * n;

				std::complex<double>tmp1 = tmp[index1];
				std::complex<double>tmp2 = tmp[index2];
				std::complex<double>tmp3 = { std::cos(2 * snd::PI() * p / data.size()), std::sin(2 * snd::PI() * p / data.size()) };

				tmp[index1] = tmp1 + tmp2;
				if (st < stage)
				{
					tmp[index2].real((tmp1.real() - tmp2.real()) * tmp3.real() - (tmp1.imag() - tmp2.imag()) * tmp3.imag());
					tmp[index2].imag((tmp1.imag() - tmp2.imag()) * tmp3.real() + (tmp1.real() - tmp2.real()) * tmp3.imag());
				}
				else
				{
					tmp[index2] = tmp1 - tmp2;
				}
			}

			unsigned int m = unsigned int(std::pow(2, st - 1)) + i;
			index[m] = index[i] + unsigned int(std::pow(2, stage - st));
		}
	}

	std::vector<double>real(data.size());

	//���ёւ�
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		if (index[i] > i)
		{
			std::swap(tmp[index[i]], tmp[i]);
		}

		tmp[i] /= data.size();
		real[i] = tmp[i].real();
	}

	size_t n = (num < 0) ? data.size() - 1 : num;

	return std::vector<double>(&real[0], &real[n]);
}
