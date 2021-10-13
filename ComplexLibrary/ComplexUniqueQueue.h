#pragma once

#include "ComplexQueue.h"
#include "ComplexLock.h"

namespace ComplexLibrary
{
	template <typename T>
	class ComplexUniqueQueue
	{
	public:

		ComplexUniqueQueue()
		{

		}

		virtual ~ComplexUniqueQueue()
		{

		}

		void push(T value)
		{
			m_queue.push(value);
		}

		T& front() const
		{
			return m_queue.front();
		}

		T& back() const
		{
			return m_queue.back();
		}

		void pop()
		{
			m_queue.pop();
		}

		int size()
		{
			return m_queue.size();
		}

		bool empty()
		{
			return m_queue.empty();
		}

		void clear()
		{
			m_queue.clear();
		}

		void lock()
		{
			m_lock.Lock();
		}

		void unlock()
		{
			m_lock.UnLock();
		}

		bool trylock()
		{
			return m_lock.TryLock();
		}

		ComplexLock* getlock()
		{
			return &m_lock;
		}

	private:

		ComplexQueue<T> m_queue;
		ComplexLock m_lock;

	};


	template <typename T>
	class ComplexUniqueSwapQueue
	{
	public:

		ComplexUniqueSwapQueue()
		{
			m_inputQueue = &m_queue[0];
			m_outputQueue = &m_queue[1];
		}

		virtual ~ComplexUniqueSwapQueue() {}

		void push(T value) { m_inputQueue->push(value); }
		T& front() const { return m_inputQueue->front(); }
		T& back() const { return m_inputQueue->back(); }
		void pop() { m_inputQueue->pop(); }
		int size() { return m_inputQueue->size(); }
		bool empty() { return m_inputQueue->empty(); }
		void clear() { m_inputQueue->clear(); }

		void pushOutputQueue(T value) { m_outputQueue->push(value); }
		T& frontOutputQueue() const { return m_outputQueue->front(); }
		T& backOutputQueue() const { return m_outputQueue->back(); }
		void popOutputQueue() { m_outputQueue->pop(); }
		int sizeOutputQueue() { return m_outputQueue->size(); }
		bool emptyOutputQueue() { return m_outputQueue->empty(); }
		void clearOutputQueue() { m_outputQueue->clear(); }

		void swapQueue()
		{
			m_lock.Lock();

			if (m_inputQueue == &m_queue[0])
			{
				m_inputQueue = &m_queue[1];
				m_outputQueue = &m_queue[0];
			}
			else
			{
				m_inputQueue = &m_queue[0];
				m_outputQueue = &m_queue[1];
			}

			m_lock.UnLock();
		}

		void lock() { m_lock.Lock(); }
		void unlock() { m_lock.UnLock(); }
		bool trylock() { return m_lock.TryLock(); }
		ComplexLock* getlock() { return &m_lock; }

	private:

		ComplexQueue<T> m_queue[2];
		ComplexQueue<T>* m_inputQueue;
		ComplexQueue<T>* m_outputQueue;
		ComplexLock m_lock;

	};
}