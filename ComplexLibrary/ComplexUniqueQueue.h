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
}