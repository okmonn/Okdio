#include "WAVE.h"
#include "../SoundInfo.h"
#include <algorithm>

// char型オーバーフローの防止
#define OVERFLLOW_CHAR 127.0f
// short型オーバーフローの防止
#define OVERFLLOW_SHORT 32768.0f

// RIFF読み込み
int LoadRIFF(wav::RIFF& riff, FILE* file)
{
	fread(&riff, sizeof(riff), 1, file);

	std::string id = (char*)(riff.id);
	std::string type = (char*)(riff.type);
	if (id.find("RIFF") == std::string::npos
		|| type.find("WAVE") == std::string::npos)
	{
		fclose(file);
		return -1;
	}

	return 0;
}

// FMT読み込み
int LoadFMT(wav::FMT& fmt, FILE* file)
{
	fread(&fmt, sizeof(fmt), 1, file);

	std::string id = (char*)(fmt.id);
	if (id.find("fmt ") == std::string::npos)
	{
		fclose(file);
		return -1;
	}

	//拡張部分の読み込み
	std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
	fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, file);

	return 0;
}

// DATA読み込み
int LoadData(wav::DATA& data, FILE* file)
{
	//ダミー宣言 
	std::string chunkID("1234");
	while (true)
	{
		fread(&chunkID[0], sizeof(unsigned char) * chunkID.size(), 1, file);

		//DATAチャンク発見
		if (chunkID == "data")
		{
			break;
		}
		//その他のチャンク
		else
		{
			unsigned long size = 0;
			fread(&size, sizeof(size), 1, file);

			std::vector<unsigned char>data(size);
			fread(data.data(), sizeof(unsigned char) * data.size(), 1, file);
		}
	}

	//DATAの読み込み 
	data.id = chunkID;
	fread(&data.size, sizeof(data.size), 1, file);

	return 0;
}

// 8ビット読み込み
void Load8(std::vector<float>& data, FILE* file)
{
	std::vector<unsigned char>tmp(data.size());
	fread(tmp.data(), sizeof(tmp[0]) * tmp.size(), 1, file);

	data.assign(tmp.begin(), tmp.end());
	for (float& i : data)
	{
		i = (i / OVERFLLOW_CHAR) - 1.0f;
	}
}

// 16ビット読み込み
void Load16(std::vector<float>& data, FILE* file)
{
	std::vector<short>tmp(data.size());
	fread(tmp.data(), sizeof(tmp[0]) * tmp.size(), 1, file);

	data.assign(tmp.begin(), tmp.end());
	for (float& i : data)
	{
		i = (i / OVERFLLOW_SHORT);
	}
}

// 読み込み
int wav::Load(const std::string& fileName, snd::Info& info, std::shared_ptr<std::vector<float>>& outData)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	RIFF riff{};
	LoadRIFF(riff, file);

	FMT fmt{};
	LoadFMT(fmt, file);

	DATA data{};
	LoadData(data, file);

	info.sample  = fmt.sample;
	info.bit     = fmt.bit;
	info.channel = fmt.channel;

	outData = std::make_shared<std::vector<float>>(data.size / (fmt.bit / 8));

	long size = data.size / (fmt.bit / 8);
	switch (fmt.bit)
	{
	case 8:
		Load8(*outData, file);
		break;
	case 16:
		Load16(*outData, file);
		break;
	default:
		break;
	}

	fclose(file);

	return 0;
}
