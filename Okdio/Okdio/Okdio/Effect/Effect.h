#pragma once
#include <vector>

class Okdio;
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
	virtual void Execution(Okdio* okdio) {
	}
};
