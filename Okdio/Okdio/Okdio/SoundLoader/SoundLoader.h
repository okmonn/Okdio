#pragma once
#include "../SoundInfo.h"
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

namespace snd {
	struct Info;
}

// サウンドファイル読み込み
class SoundLoader
{
public:
	// デストラクタ
	~SoundLoader();

	// 読み込み
	int Load(const std::string& fileName, snd::Info& info);

	// インスタンス変数取得
	static SoundLoader& Get(void);
	// サウンド情報取得
	snd::Info Info(const std::string& fileName);
	// サウンドデータ取得
	std::shared_ptr<std::vector<float>> Wave(const std::string& fileName);

private:
	// コンストラクタ
	SoundLoader();
	SoundLoader(const SoundLoader&) = delete;
	void operator=(const SoundLoader&) = delete;

	// 初期化
	void Init(void);


	// サウンド情報
	std::unordered_map<std::string, snd::Info>info;

	// 波形データ
	std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>wave;

	// 関数ポインタ
	std::unordered_map<std::string, std::function<int(const std::string&, snd::Info&)>>func;
};
