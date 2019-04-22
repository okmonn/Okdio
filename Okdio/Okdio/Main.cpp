#include "Okdio/Okdio.h"
#include "Okdio/Effector.h"
#include <Windows.h>
#define KEY

int main()
{
	//エフェクターの生成
	Effector effe(10, 2);
	//ライブラリ本体
	Okdio s("mtgx2.wav", &effe);
	//エフェクト生成
	TimeStretch time(2.0f);
	//エフェクトの追加
	//s.PushEffect(&time);
	//再生
	s.Play();

	//再生終了までループ
#ifdef KEY
	while (!(GetKeyState(VK_ESCAPE) & 0x80))
#else
	while(!s.IsPlayEnd())
#endif
	{

	}

	s.Stop();

	//波形データ削除
	snd::Delete(s.GetName());

	return 0;
}
