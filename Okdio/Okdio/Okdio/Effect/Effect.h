#pragma once
#include <vector>

class Effector;

class Effect
{
	friend Effector;
public:
	// �R���X�g���N�^
	Effect() {
	}
	// �f�X�g���N�^
	virtual ~Effect() {
	}

private:
	// ���s
	virtual void Execution(std::vector<float>& data) {
	}
};
