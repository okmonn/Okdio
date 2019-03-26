#pragma once
#include <vector>

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

protected:

private:
	// 実行
	virtual void Execution(std::vector<float>& data) = delete;
};
