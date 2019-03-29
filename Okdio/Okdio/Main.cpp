#include "Okdio/OKdio.h"
#include "Okdio/Effector.h"
#include "Okdio/Effects.h"
#include <Windows.h>

int main()
{
	Effector effe(10, 2);
	Pan pan(0.0f);
	Okdio s("mtgx.wav", &effe);
	s.PushEffect(&pan);
	//s.SetEffect({ &vol, &dis, &low });
	s.Play(false, 10);
	
	float angle = 0.0f;
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

		const float tmp = 0.05f;
		if (GetKeyState(VK_RIGHT) & 0x80)
		{
			angle += tmp;
			if (angle > 90.0f)
			{
				angle = 90.0f;
			}
			pan.SetPanning(angle);
		}
		else if (GetKeyState(VK_LEFT) & 0x80)
		{
			angle -= tmp;
			if (angle < -90.0f)
			{
				angle = -90.0f;
			}
			pan.SetPanning(angle);
		}
	}

	return 0;
}
