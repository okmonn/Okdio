#pragma once
#include <bitset>

class Fix
{
public:
	// コンストラクタ
	Fix() : exponent(0) {
		integer = 1;
		decimal = 0;
	}
	Fix(const float& f) : exponent(0) {
		integer = 1;
		decimal = 0;
		Convert(f);
	}
	// コピーコンストラクタ
	Fix(const Fix& fix) {
		(*this) = fix;
	}
	// デストラクタ
	~Fix() {}

	// floatに変換
	float Float(const unsigned int& integer = 8, const unsigned int& decimal = 15)
	{
		if (integer + decimal != 23)
		{
			return 0.0f;
		}

		unsigned int index = 0;
		std::bitset<32>fl(0);

		//整数部ビット
		int integerBit  = 0;
		for (unsigned int i = 0; i < integer; ++i)
		{
			if (this->integer[i] == 1)
			{
				integerBit = i;
			}
		}
		//小数部ビット
		int decimalBit = 0;
		for (int i = decimal * !exponent - 1; i >= 0; --i)
		{
			if (this->decimal[i] == 1)
			{
				decimalBit = i;
			}
		}

		//小数部
		for (int i = (decimal + decimalBit) + (integer - integerBit) - exponent; i >= decimalBit + !exponent; --i)
		{
			fl[index++] = this->decimal[i];
		}
		//整数部
		for (int i = 0; i < integerBit; ++i)
		{
			fl[index++] = this->integer[i];
		}

		//指数部
		std::bitset<8>exponent(integerBit + decimalBit + !this->exponent );
		if (this->exponent == false)
		{
			Complement(exponent);
		}
		//下駄ばき
		Add(exponent, std::bitset<8>(127));
		for (size_t i = 0; i < exponent.size(); ++i)
		{
			fl[index++] = exponent[i];
		}

		//符号部
		fl[index] = sign[0];

		float f = 0.0f;
		int* tmp = (int*)&f;
		(*tmp) = int(fl.to_ulong());

		return f;
	}

	// 演算子オーバーロード
	void operator=(const Fix& fix)
	{
		sign     = fix.sign;
		integer  = fix.integer;
		decimal  = fix.decimal;
		exponent = fix.exponent;
	}
	void operator+=(const Fix& fix)
	{
		auto up = Add(decimal, fix.decimal, true);
		Add(integer, std::bitset<32>(up[0]));

		Add(integer, fix.integer);
	}

private:
	// 加算
	template <unsigned int T>
	std::bitset<1> Add(std::bitset<T> & bitA, const std::bitset<T> & bitB, const bool& reverse = false)
	{
		std::bitset<2>sum(0);
		std::bitset<1>up(0);

		if (reverse == false)
		{
			for (unsigned int i = 0; i < T; ++i)
			{
				sum = bitA[i] + bitB[i] + up[0];
				bitA[i] = sum[0];
				up[0]   = sum[1];
			}
		} 
		else
		{
			for (int i = T - 1; i >= 0; --i)
			{
				sum = bitA[i] + bitB[i] + up[0];
				bitA[i] = sum[0];
				up[0]   = sum[1];
			}
		}

		return up;
	}

	// 2の補数
	template <unsigned int T>
	void Complement(std::bitset<T>& bit)
	{
		bit = ~bit;
		Add(bit, std::bitset<T>(1));
	}

	// floatを変換
	void Convert(const float& f)
	{
		//floatの要素（符号部、指数部、仮数部）を分解
		int tmp = *(int*)&f;
		sign = (tmp >> 31) & 1;
		std::bitset<8>exponent(((tmp >> 23) & 0xff) - 127);
		std::bitset<23>fraction(tmp & 0x7fffff);

		//指数部正数
		if (this->exponent = exponent[7] == 0)
		{
			size_t index = fraction.size() - 1;
			//整数部取得
			for (size_t i = 0; i < size_t(exponent.to_ulong()); ++i)
			{
				integer <<= 1;
				integer[0] = fraction[index--];
			}
			//小数部取得
			for (size_t i = 0; i < fraction.size() - size_t(exponent.to_ulong()); ++i)
			{
				decimal[i] = fraction[index--];
			}
		}
		//指数部負数
		else
		{
			Complement(exponent);
			//小数部取得
			for (size_t i = 0; i < fraction.size(); ++i)
			{
				decimal <<= 1;
				decimal[0] = fraction[i];
			}
			//整数部取得
			for (size_t i = 0; i < size_t(exponent.to_ulong()); ++i)
			{
				decimal <<= 1;
				decimal[0] = integer[0];
				integer >>= 1;
			}
		}
	}


	// 符号部
	std::bitset<1>sign;

	// 整数部
	std::bitset<32>integer;

	// 小数部
	std::bitset<32>decimal;

	// 指数部正数フラグ
	char exponent;
};
