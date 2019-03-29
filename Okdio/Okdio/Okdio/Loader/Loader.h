#pragma once
#include "../etc/Info.h"
#include <string>
#include <memory>
#include <functional>
#include <unordered_map>

// 音声ファイル読み込み
class Loader
{
public:
	// デストラクタ
	~Loader();

	// ファイル読み込み
	int Load(const std::string& fileName);

	// オリジナル波形登録
	std::string Create(const snd::Info& info, const std::vector<float>& data);

	// サウンドデータ削除
	void Delete(const std::string& fileName);

	// インスタンス変数取得
	static Loader& Get(void);

	// サウンド情報取得
	snd::Info Info(const std::string& fileName);

	// 波形データ取得
	std::weak_ptr<std::vector<float>> Data(const std::string& fileName);

private:
	// コンストラクタ
	Loader();
	Loader(const Loader&) = delete;
	void operator=(const Loader&) = delete;

	// 初期化
	void Init(void);


	// オリジナル波形判別名
	std::string original;

	// オリジナル登録ナンバー
	unsigned int cnt;

	// サウンド情報
	std::unordered_map<std::string, snd::Info>info;

	// 波形データ
	std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>data;

	// 関数ポインタ
	std::unordered_map<std::string, std::function<int(const std::string&)>>func;
};
