#include "Okdio/OKdio.h"

int main()
{
	Effector effe(10, 2);

	Okdio s;
	snd::Info info = { 44100, 16, 1 };
	std::vector<float>a(info.sample * info.channel * 1);
	for (unsigned int i = 0; i < a.size(); ++i)
	{
		a[i] = 1.0f * std::sin(2.0f * 3.14f * 440.0f * i / info.sample);
	}

	//s.CreateOriginal(info, a);
	s.Load("mtgx.wav");
	auto z = s;
	z.Play(false);
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{

	}
}
