#pragma once
#include "../etc/Singleton.h"

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2 : 
	public Singleton<XAudio2>
{
	friend Singleton<XAudio2>;
public:
	// オーディオ取得
	IXAudio2* Audio(void) const;

	// マスタリングボイス取得
	IXAudio2MasteringVoice* Mastering(void) const;

private:
	// コンストラクタ
	XAudio2();
	// デストラクタ
	~XAudio2();

	// COM初期化
	void InitCom(void);

	// オーディオ生成
	void CreateAudio(void);

	// マスタリングボイスの生成
	void CreateMastering(void);


	// オーディオ
	IXAudio2* audio;

	// マスタリングボイス
	IXAudio2MasteringVoice* mastering;
};
