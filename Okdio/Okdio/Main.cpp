#include "Okdio/Okdio.h"
#include <iostream>
#include <Windows.h>

bool now = false;
bool old = false;

bool IsKey(const unsigned char& key)
{
	old = now;
	now = GetKeyState(key) & 0x80;
	return now;
}
bool IsTrigger(const unsigned char& key)
{
	return IsKey(key) && old != now;
}

#include "Okdio/etc/Fix.h"
int main()
{
	Fix a(0.035191f);
	Fix b(0.5f);
	//a += b;
	printf("%f\n", a.Float());


	Okdio okdio;
	auto& mic = Microphone::Get();
	mic.SetAudio(&okdio);
	mic.Start();
	//okdio.SetFilter(snd::FilterType::LowPass, 100.0f);
	okdio.Play(true);

	while (IsKey(VK_ESCAPE) == false)
	{
	}

	return 0;
}