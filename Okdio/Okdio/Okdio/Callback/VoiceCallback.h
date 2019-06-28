#pragma once
#include <xaudio2.h>

class Okdio;

class VoiceCallback : 
	public IXAudio2VoiceCallback
{
public:
	// コンストラクタ
	VoiceCallback(Okdio* okdio);
	// デストラクタ
	~VoiceCallback();

private:
	VoiceCallback(const VoiceCallback&) = delete;
	void operator=(const VoiceCallback&) = delete;

	// データ読み込み前に呼び出し
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired);

	// 新しいバッファの処理開始時に呼び出し
	void __stdcall OnBufferStart(void* pBufferContext);

	// バッファの処理終了時に呼び出し
	void __stdcall OnBufferEnd(void* pBufferContext);

	// 音声の処理パス終了時に呼び出し
	void __stdcall OnVoiceProcessingPassEnd(void);

	// 連続したストリーム再生終了時に呼び出し
	void __stdcall OnStreamEnd(void) {}

	// ループ終了位置到達時に呼び出し
	void __stdcall OnLoopEnd(void* pBufferContext) {}

	// エラー発生時に呼び出し
	void __stdcall OnVoiceError(void* pBufferContext, long Error) {}


	// サウンドライブラリ
	Okdio* okdio;
};
