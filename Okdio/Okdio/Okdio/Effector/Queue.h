#pragma once
#include <deque>

// ため込み用キュー
template <typename T>
class Queue
{
public:
	// コンストラクタ
	Queue(const unsigned int& capacity) {
		this->capacity = capacity;
	}
	// デストラクタ
	~Queue() {
	}

	// キュー追加
	bool Push(T&& input) {
		if (queue.size() >= capacity)
		{
			return false;
		}

		queue.push_back(std::move(input));

		return true;
	}

	// キュー追加
	bool Push(const T& input) {
		if (queue.size() >= capacity)
		{
			return false;
		}

		queue.push_back(input);

		return true;
	}

	// キュー吐き出し
	bool Pop(T& output) {
		if (queue.empty())
		{
			return false;
		}

		output = std::move(queue.front());
		queue.pop_front();

		return true;
	}

	// キューが空か判定
	bool Empty(void) const {
		return queue.empty();
	}

private:
	// ため込み容量
	unsigned int capacity;

	// キュー
	std::deque<T>queue;
};
