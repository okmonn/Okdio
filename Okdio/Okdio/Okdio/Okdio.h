#pragma once
#include "MixVoice/MixVoice.h"
#include "etc/SoundEffects.h"
#include "Microphone/Microphone.h"
#include <string>
#include <memory>

class VoiceCallback;

class Okdio : 
	public Voice<IXAudio2SourceVoice>
{
	friend VoiceCallback;
public:
	// コンストラクタ
	Okdio();
	Okdio(const std::string& fileName, MixVoice* mix = nullptr, const size_t& num = 0);
	Okdio(const snd::Info& info, const std::vector<float>& data);
	// デストラクタ
	~Okdio();

	// 読み込み
	void Load(const std::string& fileName, MixVoice* mix = nullptr, const size_t& num = 0);

	// カスタム波形登録
	void CustomWave(const snd::Info& info, const std::vector<float>& data);

	// 再生
	void Play(const bool& loop = false);

	// 停止
	void Stop(void);

	// 再生速度調節
	bool SetSpeed(const float& rate);

	// 登録名取得
	std::string Name(void) const;

	// 読み込み位置取得
	size_t& Read(void);

	// 代入演算子オーバーロード
	void operator=(const Okdio& okdio);

private:
	// 初期化
	void Init(void);

	// ソースボイス生成
	void CreateVoice(void);

	// 波形データをボイスバッファに追加
	void Submit(void);

	// 終了確認
	void CheckEnd(void);

	// ミックスボイスセット
	void SetMixVoice(MixVoice* mix, const size_t& num);


	// コールバック
	std::unique_ptr<VoiceCallback>back;

	// サウンド参照名
	std::string name;

	// 読み込み位置
	size_t read;

	// ループフラグ
	bool loop;

	// 再生終了フラグ
	bool end;
};
