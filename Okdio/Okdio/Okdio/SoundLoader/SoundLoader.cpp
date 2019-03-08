#include "SoundLoader.h"
#include "../WAVE/WAVE.h"

// コンストラクタ
SoundLoader::SoundLoader()
{
	Init();
}

// デストラクタ
SoundLoader::~SoundLoader()
{
}

// 初期化
void SoundLoader::Init(void)
{
	func[".wav"] = [&](const std::string& fileName, snd::Info& info)->int {
		return wav::Load(fileName, info, wave[fileName]);
	};
}

// 読み込み
int SoundLoader::Load(const std::string& fileName, snd::Info& info)
{
	std::string format = fileName.substr(fileName.find_last_of("."), fileName.size());
	if (func.find(format) == func.end())
	{
		return -1;
	}

	return func[format](fileName, info);
}

// インスタンス変数取得
SoundLoader& SoundLoader::Get(void)
{
	static SoundLoader instance;
	return instance;
}

// サウンド情報取得
snd::Info SoundLoader::Info(const std::string& fileName)
{
	if (info.find(fileName) == info.end())
	{
		return snd::Info(UINT_MAX);
	}

	return info[fileName];
}

// サウンドデータ取得
std::shared_ptr<std::vector<float>> SoundLoader::Wave(const std::string& fileName)
{
	if (wave.find(fileName) == wave.end())
	{
		return nullptr;
	}

	return wave[fileName];
}
