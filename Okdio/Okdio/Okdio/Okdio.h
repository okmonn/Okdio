#pragma once
#include "SoundInfo.h"
#include <list>
#include <string>
#include <vector>
#include <optional>
#include <xaudio2.h>

class Effector;

// サウンド
class Okdio : 
	public IXAudio2VoiceCallback
{
	friend Effector;
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
	long Play(const bool& loop = false, const bool& multiple = false);

	// 停止
	long Stop(void);

	// 代入演算子
	void operator=(const Okdio& okdio);

private:
	// 初期化
	void Init(void);

	// ソースボイス生成
	long CreateVoice(void);

	// 波形更新
	void UpData(void);

	// バッファに追加
	long Submit(void);

	// 終了確認
	void CheckEnd(void);

	// リセット
	void Reset(void);

	// 一回の処理データサイズ
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

	// バッファ入れ替え用インデックス
	unsigned int index;

	// 再生呼び出し回数
	unsigned int cnt;

	// ループフラグ
	bool loop;

	// 読み込み位置
	std::list<unsigned int>read;

	// 波形データ
	std::vector<std::vector<float>>wave;

	void* handle;
};
