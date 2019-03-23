#include "Okdio/OKdio.h"

int main()
{
	Okdio s;
	snd::Info info = { 44100, 16, 1 };
	std::vector<float>a(info.sample * info.channel * 1);
	for (unsigned int i = 0; i < a.size(); ++i)
	{
		a[i] = 1.0f * std::sin(2.0f * 3.14f * 440.0f * i / info.sample);
	}

	//s.CreateOriginal(info, a);
	s.Load("mtgx.wav");
	s.Play(false);
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{

	}
}
