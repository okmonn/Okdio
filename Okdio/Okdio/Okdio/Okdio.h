#pragma once
#include "etc/Info.h"
#include "etc/Func.h"
#include <string>
#include <vector>
#include <memory>

struct IXAudio2SourceVoice;
class VoiceCallback;
class Effector;
class Effect;

// サウンドライブラリ
class Okdio
{
	friend VoiceCallback;
	friend Effector;
public:
	// コンストラクタ
	Okdio();
	Okdio(Effector* effector);
	Okdio(const std::string& fileName);
	Okdio(const snd::Info& info, const std::vector<float>& data);
	Okdio(const std::string& fileName, Effector* effector);
	Okdio(const snd::Info& info, const std::vector<float>& data, Effector* effector);
	// コピーコンストラクタ
	Okdio(const Okdio& okdio);
	// デストラクタ
	~Okdio();

	// ファイルから読み込み
	int Load(const std::string& fileName);

	// オリジナル波形生成
	int CreateOriginal(const snd::Info & info, const std::vector<float>& data);

	// エフェクトを末尾に追加
	void PushEffect(Effect* effect);

	// エフェクトをまとめてセット
	void SetEffect(const std::initializer_list<Effect*>& effect);

	// 再生
	// loop　false：ループしない、true：ループする
	// overlaidMax　ディレイの重ね掛け最大値を設定してください
	long Play(const bool& loop = false, const size_t& overlaidMax = 10);

	// 停止
	long Stop(void);

	// 代入演算子オーバーロード
	void operator=(const Okdio& okdio);

	// サウンド情報取得
	snd::Info GetInfo(void) const;

	// 再生終了確認
	bool IsPlayEnd(void) const;

	// 参照ファイルパス取得
	std::string GetName(void) const;

	// 現在の波形情報取得
	std::vector<float>& GetData(void);

	// フィルタ用入出力データ取得
	float* GetInOut(void);

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

	// 一回の処理データ取得
	inline size_t Bps(void) const;


	// ボイスコールバック
	std::unique_ptr<VoiceCallback>back;

	// エフェクター
	Effector* effector;

	// 参照ファイル名
	std::string name;

	// イベントハンドル
	void* handle;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// 再生終了フラグ
	bool endFlag;

	// ループフラグ
	bool loop;

	// 重ねがけ回数
	size_t cnt;

	// バッファ入れ替え用インデックス
	unsigned int index;

	// フィルタ用入出力
	float inout[4];

	// 波形読み込み位置
	std::vector<size_t>read;

	// 現在の波形データ
	std::vector<std::vector<float>>data;

	// エフェクト
	std::vector<Effect*>effect;
};
