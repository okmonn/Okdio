#pragma once
#include "etc/Voice.h"
#include "Func/Func.h"
#include <memory>

class VoiceCallback;
class Manager;

class __declspec(uuid("C1FFFB4C-EC30-49DF-84ED-FD90BC1FC2AA"))
Okdio : public Voice<Okdio, IXAudio2SourceVoice>
{
	friend Voice<Okdio, IXAudio2SourceVoice>;
	friend VoiceCallback;
	friend Manager;
public:
	// 読み込み
	long Load(const std::string& fileName);

	// 再生
	long Play(const bool& loop = false);

	// 停止
	long Stop(void);

private:
	// コンストラクタ
	Okdio();
	Okdio(const Okdio&) = delete;
	void operator=(const Okdio&) = delete;
	// デストラクタ
	~Okdio();

	// ソースボイス生成
	long CreateVoice(void);

	// オーディオバッファに追加
	void Submit(void);

	// 終了チェック
	void CheckEnd(void);


	// コールバック
	std::unique_ptr<VoiceCallback>callback;

	// 参照ファイル名
	std::string name;

	// 読み込み位置
	size_t read;

	// ループフラグ
	bool loop;

	// 終了フラグ
	bool end;
};
