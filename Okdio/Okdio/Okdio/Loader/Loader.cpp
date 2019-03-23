#include "Loader.h"
#include "../etc/WaveFmt.h"

// コンストラクタ
Loader::Loader() : 
	original("original"), cnt(0)
{
	Init();
}

// デストラクタ
Loader::~Loader()
{
}

// 初期化
void Loader::Init(void)
{
	func[".wav"] = [&](const std::string& fileName)->int {
		if (info.find(fileName) != info.end())
		{
			return 0;
		}

		int hr = wav::Load(fileName, info[fileName], data[fileName]);
		if (hr != 0)
		{
			info.erase(info.find(fileName));
			data.erase(data.find(fileName));
		}

		return hr;
	};
}

// ファイル読み込み
int Loader::Load(const std::string& fileName)
{
	std::string format = fileName.substr(fileName.find_last_of("."), fileName.size());
	if (func.find(format) == func.end())
	{
		return -1;
	}

	return func[format](fileName);
}

// オリジナル波形登録
std::string Loader::Create(const snd::Info& info, const std::vector<float>& data)
{
	original += cnt++;
	this->info[original] = info;
	this->data[original] = std::make_shared<std::vector<float>>(data);

	return original;
}

// インスタンス変数取得
Loader& Loader::Get(void)
{
	static Loader instance;
	return instance;
}

// サウンド情報取得
snd::Info Loader::Info(const std::string& fileName)
{
	if (info.find(fileName) == info.end())
	{
		return snd::Info();
	}

	return info[fileName];
}

// 波形データ取得
std::weak_ptr<std::vector<float>> Loader::Data(const std::string& fileName)
{
	if (data.find(fileName) == data.end())
	{
		return std::weak_ptr<std::vector<float>>();
	}

	return data[fileName];
}
