#pragma once
#include "etc/Info.h"
#include <string>
#include <vector>
#include <xaudio2.h>

// サウンドライブラリ
class Okdio :
	IXAudio2VoiceCallback
{
public:
	// コンストラクタ
	Okdio();
	Okdio(const std::string& fileName);
	Okdio(const snd::Info& info, const std::vector<float>& data);
	// デストラクタ
	~Okdio();

	// ファイルから読み込み
	int Load(const std::string& fileName);

	// オリジナル波形生成
	int CreateOriginal(const snd::Info & info, const std::vector<float>& data);

	// 再生
	long Play(const bool& loop = false);

	// 停止
	long Stop(void);

private:
	// 初期化
	void Init(void);

	// ソースボイス生成
	long CreateVoice(void);

	// 波形データをボイスバッファに追加
	long Submit(void);

	// 波形データ更新
	void UpData(void);

	// 終了確認
	void CheckEnd(void);

	// リセット
	void Reset(void);

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

	// 一回の処理データ取得
	inline long Bps(void) const;


	// 参照ファイル名
	std::string name;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// ループフラグ
	bool loop;

	// 重ねがけ回数
	unsigned int cnt;

	// バッファ入れ替え用インデックス
	unsigned int index;

	// 波形読み込み位置
	std::vector<long>read;

	// 現在の波形データ
	std::vector<std::vector<float>>data;
};
