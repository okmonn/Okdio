#pragma once
#include "../etc/Singleton.h"
#include "../etc/Info.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

struct IMFAttributes;

class Loader : 
	public Singleton<Loader>
{
	friend Singleton<Loader>;
public:
	// 読み込み
	int Load(const std::string& fileName);

	// カスタム波形登録
	std::string Custom(const snd::Info& info, const std::vector<float>& data);

	// 登録波形の変更
	bool Change(const std::string& fileName, const std::vector<float>& data);

	// 波形データ削除
	void Delete(const std::string& fileName);

	// ファイルパスの確認
	bool IsFilePath(const std::string& fileName);

	// サウンド情報取得
	snd::Info Info(const std::string& fileName);

	// 波形データ取得
	std::weak_ptr<std::vector<float>> Data(const std::string& fileName);

	// 波形データ数取得
	size_t DataSize(const std::string& fileName);

	// 1回の処理データ量取得
	size_t Size(const std::string& fileName);

private:
	// コンストラクタ
	Loader();
	// デストラクタ
	~Loader();

	// WMF初期化
	void InitWMF(void);

	// 初期化
	void Init(void);

	// 圧縮音声データの読み込み
	long LoadWMF(const std::string& fileName);


	// カスタム波形識別名
	std::string custom;

	// カスタム波形登録数
	unsigned char cnt;

	// 属性値
	IMFAttributes* attribute;

	// サウンド情報
	std::unordered_map<std::string, snd::Info>info;

	// 波形データ
	std::unordered_map<std::string, std::shared_ptr<std::vector<float>>>data;

	// 関数ポインタ
	std::unordered_map<std::string, std::function<int(const std::string&)>>func;
};
