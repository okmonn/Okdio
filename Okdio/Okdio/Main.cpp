#include "Okdio/Okdio.h"
#include <vector>
#include <Windows.h>

int main()
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	okmonn::EnginStart();
	std::vector<Okdio*>okdio(2, nullptr);
	for (auto& i : okdio)
	{
		okmonn::CreateObj(IID_PPV_ARGS(&i));
	}
	okdio[0]->Load("INSIDE.wav");
	okdio[0]->Play(true);
	okdio[1]->Load("Demo1.wav");
	okdio[1]->Play(true);
	
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{
		if (GetKeyState(VK_RETURN) & 0x80)
		{
			if (okdio[0] != nullptr)
			{
				okdio[0]->Release();
				okdio[0] = nullptr;
			}
		}
	}

	for (auto& i : okdio)
	{
		if (i != nullptr)
		{
			i->Release();
		}
	}
	okmonn::EnginEnd();
	CoUninitialize();
	return 0;
}
