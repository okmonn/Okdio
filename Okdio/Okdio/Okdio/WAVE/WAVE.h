#pragma once
#include <string>
#include <vector>
#include <memory>

namespace snd {
	struct Info;
}

// WAVE�t�H�[�}�b�g�֌W
namespace wav
{
	// RIFF
	struct RIFF {
		//ID
		unsigned char id[4];
		//�T�C�Y
		long size;
		//�^�C�v
		unsigned char type[4];
	};

	// FMT
	struct FMT {
		//ID
		unsigned char id[4];
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

	// DATA
	struct DATA {
		//ID
		std::string id;
		//�T�C�Y
		long size;
	};

	// �ǂݍ���
	int Load(const std::string& fileName, snd::Info& info, std::shared_ptr<std::vector<float>>& outData);
}
