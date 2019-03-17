#include "Okdio/Okdio.h"
#include <Windows.h>

int main()
{
	std::shared_ptr<Effector>effe(new Effector(2, 10));

	snd::Info info(44100, 16, 1);
	std::vector<float>a(info.sample * info.channel);
	for (unsigned int i = 0; i < a.size(); ++i)
	{
		a[i] = 1.0f * std::sin(2.0f * 3.14f * 440.0f * i / info.sample);
	}

	Okdio s(effe);
	s.Load("sample.wav");
	s.SetFilterParam(100.0f);
	s.AddEffect(snd::Effect::LowPass);
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