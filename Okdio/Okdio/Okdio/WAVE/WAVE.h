#pragma once
#include <string>
#include <vector>
#include <memory>

namespace snd {
	struct Info;
}

// WAVEフォーマット関係
namespace wav
{
	// RIFF
	struct RIFF {
		//ID
		unsigned char id[4];
		//サイズ
		long size;
		//タイプ
		unsigned char type[4];
	};

	// FMT
	struct FMT {
		//ID
		unsigned char id[4];
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

	// DATA
	struct DATA {
		//ID
		std::string id;
		//サイズ
		long size;
	};

	// 読み込み
	int Load(const std::string& fileName, snd::Info& info, std::shared_ptr<std::vector<float>>& outData);
}
