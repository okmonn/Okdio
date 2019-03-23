#pragma once
#include "Info.h"
#include <string>
#include <vector>
#include <memory>

// WAVEフォーマット関連
namespace wav
{
	// RIFF
	struct RIFF {
		//ID
		char id[4];
		//サイズ
		long size;
		//タイプ
		char type[4];
	};

	// FMT
	struct FMT {
		//ID
		char id[4];
		//サイズ
		long size;
		//タイプ
		short type;
		//チャンネル
		short channel;
		//サンプリング周波数
		long sample;
		//バイト
		long byte;
		//ブロックサイズ
		short block;
		//量子化ビット数
		short bit;
	};

	// 読み込み
	int Load(const std::string& fileName, snd::Info& info, std::shared_ptr<std::vector<float>>& outData) {
		FILE* file = nullptr;
		if (fopen_s(&file, fileName.c_str(), "rb") != 0)
		{
			return -1;
		}

		RIFF riff{};
		fread(&riff, sizeof(riff), 1, file);
		std::string id(&riff.id[0], sizeof(riff.id));
		std::string type(&riff.type[0], sizeof(riff.id));
		if (id != "RIFF" || type != "WAVE")
		{
			fclose(file);
			return -1;
		}

		FMT fmt{};
		fread(&fmt, sizeof(fmt), 1, file);
		id.assign(&fmt.id[0], sizeof(fmt.id));
		if (id != "fmt ")
		{
			fclose(file);
			return -1;
		}
		//拡張部分の読み込み
		std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
		fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, file);

		while (true)
		{
			fread(&id[0], sizeof(unsigned char) * id.size(), 1, file);

			//DATAチャンク発見
			if (id == "data")
			{
				break;
			}
			//その他のチャンク
			else
			{
				unsigned long size = 0;
				fread(&size, sizeof(size), 1, file);
				fseek(file, size, SEEK_CUR);
			}
		}
		//DATAの読み込み 
		long size = 0;
		fread(&size, sizeof(size), 1, file);

		info    = snd::Info(fmt.sample, fmt.bit, fmt.channel);
		outData = std::make_shared<std::vector<float>>(size / (fmt.bit / 8));

		switch (fmt.bit)
		{
		case 8:
		{
			std::vector<unsigned char>tmp(outData->size());
			fread(tmp.data(), sizeof(tmp[0]) * tmp.size(), 1, file);

			outData->assign(tmp.begin(), tmp.end());
			for (float& i : *outData)
			{
				i = (i / float(0xff / 2)) - 1.0f;
			}
			break;
		}
		case 16:
		{
			std::vector<short>tmp(outData->size());
			fread(tmp.data(), sizeof(tmp[0]) * tmp.size(), 1, file);

			outData->assign(tmp.begin(), tmp.end());
			for (float& i : *outData)
			{
				i = (i / float(0xffff / 2));
			}
			break;
		}
		default:
			break;
		}

		fclose(file);

		return 0;
	}
}
