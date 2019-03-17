#include "SoundFunc.h"
#include <cmath>

// �~����
float snd::PI(void)
{
	return 3.14159265358979323846f;
}

// �C�ӌ��؂�̂�
float snd::Floor(const float & data, const int & num)
{
	float tmp = data;
	tmp *= std::pow(10.0f, -num - 1);
	tmp  = std::floor(tmp);
	tmp *= std::pow(10.0f, num + 1);

	return tmp;
}
