#include "Okdio/OKdio.h"
#include "Okdio/Effector.h"
#include "Okdio/Effects.h"
#include <Windows.h>

int main()
{
	Effector effe(10, 2);
	Volume vol(1.0f);
	Distortion dis(100.0f);
	Okdio s("mtgx.wav", &effe);
	//s.SetEffect({ &dis, &vol });
	s.Play();

	bool input = false;
	while (true)
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
