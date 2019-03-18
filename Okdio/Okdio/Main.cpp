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
	//s.Load("sample.wav");
	s.SetInfo(info, a);
	s.AddEffect(snd::Effect::LowPass);
	auto q = s;
	q.AddEffect(snd::Effect::Volume);
	q.SetVolume(100.0f);
	q.AddEffect(snd::Effect::Limiter);
	q.SetCompParam(1.0f);
	q.Play(true);

	bool key = false;
	float cut = 1.0f;
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

		if (GetKeyState(VK_UP) & 0x80)
		{
			cut += 0000.1f;
			if (cut > 20000.0f)
			{
				cut = 20000.0f;
			}
		}
		else if (GetKeyState(VK_DOWN) & 0x80)
		{
			cut -= 0000.1f;
			if (cut < 0.0f)
			{
				cut = 0.0f;
			}
		}

		//q.SetVolume(cut);
		printf("%f\n", cut);
	}

	return 0;
}