#pragma once
#include <deque>

// ���ߍ��ݗp�L���[
template <typename T>
class Queue
{
public:
	// �R���X�g���N�^
	Queue(const unsigned int& capacity) {
		this->capacity = capacity;
	}
	// �f�X�g���N�^
	~Queue() {
	}

	// �L���[�ǉ�
	bool Push(T&& input) {
		if (queue.size() >= capacity)
		{
			return false;
		}

		queue.push_back(std::move(input));

		return true;
	}

	// �L���[�ǉ�
	bool Push(const T& input) {
		if (queue.size() >= capacity)
		{
			return false;
		}

		queue.push_back(input);

		return true;
	}

	// �L���[�f���o��
	bool Pop(T& output) {
		if (queue.empty())
		{
			return false;
		}

		output = std::move(queue.front());
		queue.pop_front();

		return true;
	}

	// �L���[���󂩔���
	bool Empty(void) const {
		return queue.empty();
	}

private:
	// ���ߍ��ݗe��
	unsigned int capacity;

	// �L���[
	std::deque<T>queue;
};
