#include "Manager.h"
#include "../Okdio.h"

// コンストラクタ
Manager::Manager() : 
	okdio(nullptr)
{
}

// デストラクタ
Manager::~Manager()
{
	Release();
}

// メモリ確保
void Manager::Malloc(void)
{
	if (okdio == nullptr)
	{
		okdio = new Okdio();
	}
}

// オブジェクト生成
long Manager::Create(const GUID& id, void** obj)
{
	return okdio->QueryInterface(id, obj);
}

// 解放処理
void Manager::Release(void)
{
	if (okdio != nullptr)
	{
		okdio->Release();
		okdio = nullptr;
	}
}
