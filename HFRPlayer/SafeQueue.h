#pragma once

#include <mutex>
#include <deque>

template<typename T>
class SafeQueue
{
public:
	SafeQueue() = default;
	~SafeQueue()
	{
		clear();
	}
	SafeQueue(const SafeQueue&) = delete;
	SafeQueue& operator=(const SafeQueue&) = delete;
	void enqueue(const T element);
	bool dequeue(T& out);
	void clear();
	bool isEmpty();
private:
	std::mutex m_lock;
	std::deque<T> m_elements;
};

template<typename T>
void SafeQueue<T>::enqueue(const T element)
{
	std::unique_lock<std::mutex> lock(m_lock);
	m_elements.push_back(element);
}

template<typename T>
bool SafeQueue<T>::dequeue(T& out)
{
	std::unique_lock<std::mutex> lock(m_lock);
	if (isEmpty())
	{
		return false;
	}

	out = m_elements.front();
	m_elements.pop_front();
	return true;
}

template<typename T>
void SafeQueue<T>::clear()
{
	std::unique_lock<std::mutex> lock(m_lock);
	m_elements.clear();
}

template<typename T>
inline bool SafeQueue<T>::isEmpty()
{
	return m_elements.empty();
}
