#include "Okdio/Okdio.h"
#include <Windows.h>

int main()
{
	std::vector<Okdio>s(10);
	for (auto& i : s)
	{
		i.Load("mtgx.wav");
	}
	
	for (auto& i : s)
	{
		i.Play();
		Sleep(10000);
	}

	bool key = false;
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{
		if (GetKeyState(VK_SPACE) & 0x80)
		{
			if (key == false)
			{
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