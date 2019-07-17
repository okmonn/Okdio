#include "Okdio/Okdio.h"
#include <Windows.h>

int main()
{
	auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	okmonn::EnginStart();

	Okdio* okdio = nullptr;
	okmonn::CreateObj(IID_PPV_ARGS(&okdio));
	okdio->Load("INSIDE.wav");
	okdio->Play(false);
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
	{

	}

	okdio->Release();
	okmonn::EnginEnd();
	CoUninitialize();
	return 0;
}
