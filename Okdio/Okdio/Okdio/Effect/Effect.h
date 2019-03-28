#pragma once
#include <vector>

class Okdio;
class Effector;

class Effect
{
	friend Effector;
public:
	// コンストラクタ
	Effect() {
	}
	// デストラクタ
	virtual ~Effect() {
	}

private:
	// 実行
	virtual void Execution(Okdio* okdio) {
	}
};
