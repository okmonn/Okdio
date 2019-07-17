#pragma once
#include "../etc/Singleton.h"

struct _GUID;
typedef _GUID GUID;
class Okdio;

class Manager :
	public Singleton<Manager>
{
	friend Singleton<Manager>;
public:
	// メモリ確保
	void Malloc(void);

	// オブジェクト生成
	long Create(const GUID& id, void** obj);

	// 解放処理
	void Release(void);

private:
	// コンストラクタ
	Manager();
	// デストラクタ
	~Manager();

	// サウンドライブラリ
	Okdio* okdio;
};
