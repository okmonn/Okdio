#pragma once
#include "SoundInfo.h"
#include <string>
#include <vector>
#include <optional>
#include <xaudio2.h>

class Okdio :
	public IXAudio2VoiceCallback
{
public:
	// コンストラクタ
	Okdio();
	Okdio(const std::string& fileName);
	// コピーコンストラクタ
	Okdio(const Okdio& okdio);
	// デストラクタ
	~Okdio();

	// 読み込み
	int Load(const std::string& fileName);

	// 再生
	long Play(const bool& loop = false);

	// 停止
	long Stop(void);

	// 代入
	void operator=(const Okdio& okdio);

private:
	// 初期化
	void Init(void);

	// ソースボイス生成
	long CreateVoice(void);

	// 波形データ更新
	void UpData(void);

	// バッファに追加
	long Submit(void);

	// 終了確認
	void CheckEnd(void);

	// 波形読み込みのリセット
	void Reset(void);

	// 一回の処理データ取得
	inline constexpr unsigned int Bps(void) const;

	// データ読み込み前に呼び出し
	void __stdcall OnVoiceProcessingPassStart(unsigned int SamplesRequired);
	// 新しいバッファの処理開始時に呼び出し
	void __stdcall OnBufferStart(void* pBufferContext);
	// バッファの処理終了時に呼び出し
	void __stdcall OnBufferEnd(void* pBufferContext);
	// 音声の処理パス終了時に呼び出し
	void __stdcall OnVoiceProcessingPassEnd();
	// 連続したストリーム再生終了時に呼び出し
	void __stdcall OnStreamEnd() {}
	// ループ終了位置到達時に呼び出し
	void __stdcall OnLoopEnd(void* pBufferContext) {}
	// エラー発生時に呼び出し
	void __stdcall OnVoiceError(void* pBufferContext, long Error) {}


	// ソースボイス
	IXAudio2SourceVoice* voice;

	// 参照ファイル名
	std::optional<std::string>name;

	// バッファ入れ替え用インデックス
	unsigned int index;

	// 
	unsigned int cnt;

	// ループフラグ
	bool loop;

	// エフェクト処理完了ハンドル
	void* handle;

	// サウンド情報
	snd::Info info;

	// 読み込み位置
	std::vector<unsigned int>read;

	// 波形データ
	std::vector<std::vector<float>>wave;
};
