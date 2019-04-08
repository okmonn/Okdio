#include "Okdio/Okdio.h"
#include "Okdio/Effector.h"
#include <Windows.h>

int main()
{
	//エフェクターの生成
	Effector effe(10, 2);
	//ライブラリ本体
	Okdio s("mtgx.wav", &effe);
	//エフェクト生成
	Pan pan(90.0f);
	//エフェクトの追加
	s.PushEffect(&pan);
	//再生
	s.Play();

	//再生終了までループ
	while (!s.IsPlayEnd())
	{

	}

	//波形データ削除
	snd::Delete(s.GetName());

	return 0;
}
