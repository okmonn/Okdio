#include "Okdio/OKdio.h"
#include "Okdio/Effector.h"
#include "Okdio/Effects.h"
#include <Windows.h>

int main()
{
	Effector effe(10, 2);
	Volume vol(1.0f);
	Distortion dis(100.0f);
	Filter low(snd::FilterType::LowPass, 100.0f, 1.0f / std::sqrt(2.0f));
	Okdio s("mtgx.wav");
	auto a = s.GetInfo();
	s.PushEffect(&low);
	s.SetEffect({ &vol, &dis, &low });
	s.Play(false, 10);
	auto eb = s;
	
	bool input = false;
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{
		if (GetKeyState(VK_SPACE) & 0x80)
		{
			if (input == false)
			{
				s.Play();
				input = true;
			}
		}
		else
		{
			input = false;
		}
	}

	return 0;
}
