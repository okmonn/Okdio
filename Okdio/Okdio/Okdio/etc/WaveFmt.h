#pragma once
#include "Info.h"
#include <string>
#include <vector>
#include <memory>

// WAVE�t�H�[�}�b�g�֘A
namespace wav
{
	// RIFF
	struct RIFF {
		//ID
		char id[4];
		//�T�C�Y
		long size;
		//�^�C�v
		char type[4];
	};

	// FMT
	struct FMT {
		//ID
		char id[4];
		//�T�C�Y
		long size;
		//�^�C�v
		short type;
		//�`�����l��
		short channel;
		//�T���v�����O���g��
		long sample;
		//�o�C�g
		long byte;
		//�u���b�N�T�C�Y
		short block;
		//�ʎq���r�b�g��
		short bit;
	};

	// �ǂݍ���
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
		//�g�������̓ǂݍ���
		std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
		fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, file);

		while (true)
		{
			fread(&id[0], sizeof(unsigned char) * id.size(), 1, file);

			//DATA�`�����N����
			if (id == "data")
			{
				break;
			}
			//���̑��̃`�����N
			else
			{
				unsigned long size = 0;
				fread(&size, sizeof(size), 1, file);
				fseek(file, size, SEEK_CUR);
			}
		}
		//DATA�̓ǂݍ��� 
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
