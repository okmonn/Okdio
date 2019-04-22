#pragma once
#include "Info.h"
#include <string>
#include <vector>
#include <memory>

namespace ogg
{
	// ヘッダー
	struct Header {
		//OggS
		char id[4];
		//バージョン
		char ver;
		//フラグ
		char flag;
		//終了位置
		char pos[8];
		//シリアルナンバー
		char serealNo[4];
		//ページナンバー
		char pageNo[4];
		//チェックサム
		char sum[4];
		//セグメントナンバー
		char segmentNo;
	};

	// 読み込み
	int Load(const std::string& fileName, snd::Info& info, std::shared_ptr<std::vector<float>>& data)
	{
		FILE* file = nullptr;
		if (fopen_s(&file, fileName.c_str(), "rb") != 0)
		{
			return -1;
		}

		data = std::make_shared<std::vector<float>>();
		
		//ヘッダー
		Header header{};
		fread(&header, sizeof(header), 1, file);
		std::string id(&header.id[0], sizeof(header.id));
		if (id != "OggS")
		{
			return -1;
		}

		//セグメントテーブル
		std::vector<char>table(header.segmentNo);
		fread(&table[0], sizeof(table[0]) * table.size(), 1, file);

		//データ
		for (char& i : table)
		{
			std::vector<char>tmp(i);
			fread(&tmp[0], sizeof(tmp[0]) * tmp.size(), 1, file);
		}

		return 0;
	}
}
