#pragma once
#include "Info.h"
#include <string>
#include <vector>
#include <memory>

namespace ogg
{
	// ì«Ç›çûÇ›
	int Load(const std::string& fileName, snd::Info& info, std::shared_ptr<std::vector<float>>& data)
	{
		FILE* file = nullptr;
		if (fopen_s(&file, fileName.c_str(), "rb") != 0)
		{
			return -1;
		}

		char tmp[4];
		fread(tmp, sizeof(tmp), 1, file);
		char ver = 0;
		fread(&ver, sizeof(ver), 1, file);
		char flag = 0;
		fread(&flag, sizeof(flag), 1, file);
		long serial = 0;
		fread(&serial, sizeof(serial), 1, file);
		long sequence = 0;
		fread(&sequence, sizeof(sequence), 1, file);
		long check = 0;
		fread(&check, sizeof(check), 1, file);
		char segmentNo = 0;
		fread(&segmentNo, sizeof(segmentNo), 1, file);
		int n = 0;

		return 0;
	}
}
