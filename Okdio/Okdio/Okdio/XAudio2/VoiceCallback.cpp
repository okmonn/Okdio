#include "VoiceCallback.h"
#include "../OKdio.h"

// コンストラクタ
VoiceCallback::VoiceCallback(Okdio* okdio) : 
	okdio(okdio)
{
}

// デストラクタ
VoiceCallback::~VoiceCallback()
{
}

// データ読み込み前に呼び出し
void __stdcall VoiceCallback::OnVoiceProcessingPassStart(unsigned int SamplesRequired)
{
	if (okdio->effector != nullptr)
	{
		WaitForSingleObject(okdio->handle, INFINITE);
	}

	okdio->Submit();
}

// 新しいバッファの処理開始時に呼び出し
void __stdcall VoiceCallback::OnBufferStart(void * pBufferContext)
{
	okdio->UpData();
}

// バッファの処理終了時に呼び出し
void __stdcall VoiceCallback::OnBufferEnd(void * pBufferContext)
{
	okdio->CheckEnd();
}

// 音声の処理パス終了時に呼び出し
void __stdcall VoiceCallback::OnVoiceProcessingPassEnd(void)
{
	okdio->Reset();
}
