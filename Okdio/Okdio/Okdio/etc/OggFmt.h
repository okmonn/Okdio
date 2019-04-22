#pragma once
#include "Info.h"
#include <string>
#include <vector>
#include <memory>

namespace ogg
{
	// �w�b�_�[
	struct Header {
		//OggS
		char id[4];
		//�o�[�W����
		char ver;
		//�t���O
		char flag;
		//�I���ʒu
		char pos[8];
		//�V���A���i���o�[
		char serealNo[4];
		//�y�[�W�i���o�[
		char pageNo[4];
		//�`�F�b�N�T��
		char sum[4];
		//�Z�O�����g�i���o�[
		char segmentNo;
	};

	// �ǂݍ���
	int Load(const std::string& fileName, snd::Info& info, std::shared_ptr<std::vector<float>>& data)
	{
		FILE* file = nullptr;
		if (fopen_s(&file, fileName.c_str(), "rb") != 0)
		{
			return -1;
		}

		data = std::make_shared<std::vector<float>>();
		
		//�w�b�_�[
		Header header{};
		fread(&header, sizeof(header), 1, file);
		std::string id(&header.id[0], sizeof(header.id));
		if (id != "OggS")
		{
			return -1;
		}

		//�Z�O�����g�e�[�u��
		std::vector<char>table(header.segmentNo);
		fread(&table[0], sizeof(table[0]) * table.size(), 1, file);

		//�f�[�^
		for (char& i : table)
		{
			std::vector<char>tmp(i);
			fread(&tmp[0], sizeof(tmp[0]) * tmp.size(), 1, file);
		}

		return 0;
	}
}
