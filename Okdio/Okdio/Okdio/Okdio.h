#pragma once
#include "SoundInfo.h"
#include <string>
#include <vector>
#include <optional>
#include <xaudio2.h>

// サウンド
class Okdio : 
	public IXAudio2VoiceCallback
{
public:
	// コンストラクタ
	Okdio(const std::string& fileName);
	// デストラクタ
	~Okdio();

	// 読み込み
	int Load(const std::string& fileName);

	// 再生
	long Play(const bool& loop = false);

	// 停止
	long Stop(void);

private:
	// 初期化
	void Init(void);

	// ソースボイス生成
	long CreateVoice(void);

	// バッファに追加
	long Submit(void);

	// 終了確認
	void CheckEnd(void);

	// 新しいバッファの処理開始時に呼び出し
	void __stdcall OnBufferStart(void* pBufferContext);
	// バッファの処理終了時に呼び出し
	void __stdcall OnBufferEnd(void* pBufferContext);
	// データ読み込み前に呼び出し
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired);
	// 音声の処理パス終了時に呼び出し
	void __stdcall OnVoiceProcessingPassEnd();
	// 連続したストリーム再生終了時に呼び出し
	void __stdcall OnStreamEnd();
	// ループ終了位置到達時に呼び出し
	void __stdcall OnLoopEnd(void* pBufferContext);
	// エラー発生時に呼び出し
	void __stdcall OnVoiceError(void* pBufferContext, long Error);


	// ソースボイス
	IXAudio2SourceVoice* voice;

	// 参照ファイルパス
	std::optional<std::string>name;

	// サウンド情報
	snd::Info info;

	// 配列用インデックス
	unsigned int index;

	// 再生呼び出し回数
	unsigned int cnt;

	// ループフラグ
	bool loop;

	// 一回の処理データサイズ
	unsigned int bps;

	// 読み込み位置
	std::vector<unsigned int>read;

	// 波形データ
	std::vector<std::vector<float>>wave;
};
