#pragma once
#include <bitset>

class Fix
{
public:
	// �R���X�g���N�^
	Fix() : exponent(0) {
		integer = 1;
		decimal = 0;
	}
	Fix(const float& f) : exponent(0) {
		integer = 1;
		decimal = 0;
		Convert(f);
	}
	// �R�s�[�R���X�g���N�^
	Fix(const Fix& fix) {
		(*this) = fix;
	}
	// �f�X�g���N�^
	~Fix() {}

	// float�ɕϊ�
	float Float(const unsigned int& integer = 8, const unsigned int& decimal = 15)
	{
		if (integer + decimal != 23)
		{
			return 0.0f;
		}

		unsigned int index = 0;
		std::bitset<32>fl(0);

		//�������r�b�g
		int integerBit  = 0;
		for (unsigned int i = 0; i < integer; ++i)
		{
			if (this->integer[i] == 1)
			{
				integerBit = i;
			}
		}
		//�������r�b�g
		int decimalBit = 0;
		for (int i = decimal * !exponent - 1; i >= 0; --i)
		{
			if (this->decimal[i] == 1)
			{
				decimalBit = i;
			}
		}

		//������
		for (int i = (decimal + decimalBit) + (integer - integerBit) - exponent; i >= decimalBit + !exponent; --i)
		{
			fl[index++] = this->decimal[i];
		}
		//������
		for (int i = 0; i < integerBit; ++i)
		{
			fl[index++] = this->integer[i];
		}

		//�w����
		std::bitset<8>exponent(integerBit + decimalBit + !this->exponent );
		if (this->exponent == false)
		{
			Complement(exponent);
		}
		//���ʂ΂�
		Add(exponent, std::bitset<8>(127));
		for (size_t i = 0; i < exponent.size(); ++i)
		{
			fl[index++] = exponent[i];
		}

		//������
		fl[index] = sign[0];

		float f = 0.0f;
		int* tmp = (int*)&f;
		(*tmp) = int(fl.to_ulong());

		return f;
	}

	// ���Z�q�I�[�o�[���[�h
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
	// ���Z
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

	// 2�̕␔
	template <unsigned int T>
	void Complement(std::bitset<T>& bit)
	{
		bit = ~bit;
		Add(bit, std::bitset<T>(1));
	}

	// float��ϊ�
	void Convert(const float& f)
	{
		//float�̗v�f�i�������A�w�����A�������j�𕪉�
		int tmp = *(int*)&f;
		sign = (tmp >> 31) & 1;
		std::bitset<8>exponent(((tmp >> 23) & 0xff) - 127);
		std::bitset<23>fraction(tmp & 0x7fffff);

		//�w��������
		if (this->exponent = exponent[7] == 0)
		{
			size_t index = fraction.size() - 1;
			//�������擾
			for (size_t i = 0; i < size_t(exponent.to_ulong()); ++i)
			{
				integer <<= 1;
				integer[0] = fraction[index--];
			}
			//�������擾
			for (size_t i = 0; i < fraction.size() - size_t(exponent.to_ulong()); ++i)
			{
				decimal[i] = fraction[index--];
			}
		}
		//�w��������
		else
		{
			Complement(exponent);
			//�������擾
			for (size_t i = 0; i < fraction.size(); ++i)
			{
				decimal <<= 1;
				decimal[0] = fraction[i];
			}
			//�������擾
			for (size_t i = 0; i < size_t(exponent.to_ulong()); ++i)
			{
				decimal <<= 1;
				decimal[0] = integer[0];
				integer >>= 1;
			}
		}
	}


	// ������
	std::bitset<1>sign;

	// ������
	std::bitset<32>integer;

	// ������
	std::bitset<32>decimal;

	// �w���������t���O
	char exponent;
};
