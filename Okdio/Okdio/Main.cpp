#include "Okdio/Okdio.h"
#include <Windows.h>

int main()
{
	Okdio s("mtgx.wav");
	s.Play();

	bool key = false;
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{
		if (GetKeyState(VK_SPACE) & 0x80)
		{
			if (key == false)
			{
				s.Play();  
				key = true;
			}
		}
		else
		{
			key = false;
		}
	}

	return 0;
}