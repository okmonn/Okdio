#pragma once
#include "../etc/Singleton.h"
#include "../etc/Info.h"
#include <array>
#include <vector>
#include <Windows.h>

class Okdio;

class Microphone :
	public Singleton<Microphone>
{
	friend Singleton<Microphone>;

	// 入力用構造体
	struct WAVEIN
	{
		WAVEHDR hdr;
		std::vector<short>InData;

		//コンストラクタ
		WAVEIN() {
			hdr = {};
		}
		WAVEIN(const snd::Info& info) : hdr({}) {
			Set(info);
		}

		//情報セット
		void Set(const snd::Info& info) {
			InData.resize(info.sample * info.channel);
			hdr.dwBufferLength = (DWORD)InData.size();
			hdr.lpData = (LPSTR)InData.data();
		}
	};

public:
	// 再生ターゲットセット
	void SetAudio(Okdio* okdio);

	// 入力開始
	void Start(void);

private:
	// コンストラクタ
	Microphone();
	// デストラクタ
	~Microphone();

	// 入力コールバック
	static void __stdcall WaveInProc(HWAVEIN handle, unsigned int msg, unsigned long instance, WPARAM param1, LPARAM param2);

	// 入力デバイスオープン
	void InOpen(void);

	// 入力終了
	void InFinish(void);


	// サウンドライブラリ
	static Okdio* okdio;

	// 入力ハンドル
	HWAVEIN InHandle;

	// サウンド情報
	snd::Info info;

	// 終了フラグ
	static bool fin;

	// 入力用
	static std::array<WAVEIN, 2>waveIn;

	// 再生用データ
	static std::vector<float>data;
};
