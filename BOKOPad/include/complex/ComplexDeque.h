#pragma once

#include "ComplexException.h"

namespace ComplexLibrary
{
	template <typename T>
	class ComplexDeque
	{
	public:

		ComplexDeque(int maxsize = 10)
			: m_ptr(nullptr)
			, m_maxSize(0)
			, m_size(0)
			, m_front_index(0)
			, m_back_index(0)
		{
			m_maxSize = maxsize;
			m_ptr = new T[maxsize];
		}

		ComplexDeque(ComplexDeque& other)
			: m_ptr(nullptr)
			, m_maxSize(0)
			, m_size(0)
			, m_front_index(0)
			, m_back_index(0)
		{
			m_maxSize = other.m_maxSize;
			m_size = other.m_size;
			m_front_index = other.m_front_index;
			m_back_index = other.m_back_index;
			m_ptr = new T[m_maxSize];

			for (int i = 0; i < m_maxSize; i++)
			{
				m_ptr[i] = other.m_ptr[i];
			}
		}

		~ComplexDeque()
		{
			delete[] m_ptr;
			m_ptr = nullptr;
		}

		void push_back(T value)
		{
			if (m_ptr == nullptr)
				throw ComplexNullptrException("deque member reference is null point.", "ComplexDeque", "push_back");
			if (full())
				throw ComplexCapacityOverflowException("deque capacity is overflow.", "ComplexDeque", "push_back");

			m_back_index = (m_back_index + 1) % m_maxSize;
			m_ptr[m_back_index] = value;
			m_size++;
		}

		void push_front(T value)
		{
			if (m_ptr == nullptr)
				throw ComplexNullptrException("deque member reference is null point.", "ComplexDeque", "push_front");
			if (full())
				throw ComplexCapacityOverflowException("deque capacity is overflow.", "ComplexDeque", "push_front");

			m_ptr[m_front_index] = value;
			m_front_index = (m_front_index - 1 + m_maxSize) % m_maxSize;
			m_size++;
		}

		void pop_front()
		{
			if (m_ptr == nullptr)
				throw ComplexNullptrException("deque member reference is null point.", "ComplexDeque", "pop_front");
			if (m_front_index == 0)
				throw ComplexCapacityOverflowException("deque capacity is overflow.", "ComplexDeque", "pop_front");

			m_front_index = (m_front_index + 1) % m_maxSize;
			m_size--;
		}

		void pop_back()
		{
			if (m_ptr == nullptr)
				throw ComplexNullptrException("deque member reference is null point.", "ComplexDeque", "pop_back");
			if (m_back_index == 0)
				throw ComplexCapacityOverflowException("deque capacity is overflow.", "ComplexDeque", "pop_back");

			m_back_index = (m_back_index - 1 + m_maxSize) % m_maxSize;
			m_size--;
		}

		T& at(int index)
		{
			if (m_ptr == nullptr)
				throw ComplexNullptrException("deque member reference is null point.", "ComplexDeque", "at");
			if (index < 0)
				throw ComplexIndexOutOfBoundsException("deque call index is zero or less.", "ComplexDeque", "at");
			if (index > m_maxSize)
				throw ComplexIndexOutOfBoundsException("deque call index is maxsize out of bounds.", "ComplexDeque", "at");
			if (m_size == 0)
				throw ComplexIndexOutOfBoundsException("deque size is zero.", "ComplexDeque", "at");

			int find_index = 0;
			if (m_front_index == 0)
				find_index = index + 1;
			else
				find_index = (m_front_index + 1 + index) % m_maxSize;


			return m_ptr[find_index];
		}

		T& front()
		{
			if (m_ptr == nullptr)
				throw ComplexNullptrException("deque member reference is null point.", "ComplexDeque", "front");
			if (m_front_index == 0)
				throw ComplexCapacityOverflowException("deque capacity is overflow.", "ComplexDeque", "front");

			return m_ptr[(m_front_index + 1) % m_maxSize];
		}

		T& back()
		{
			if (m_ptr == nullptr)
				throw ComplexNullptrException("deque member reference is null point.", "ComplexDeque", "back");
			if (m_back_index == 0)
				throw ComplexCapacityOverflowException("deque capacity is overflow.", "ComplexDeque", "back");

			return m_ptr[m_back_index];
		}

		int size() const
		{
			return m_size;
		}

		bool empty()
		{
			return m_size == 0;
		}

		bool full()
		{
			return m_size == m_maxSize;
		}

		void clear()
		{
			m_front_index = 0;
			m_back_index = 0;
			m_size = 0;
		}

		T& operator [] (int index)
		{
			int find_index = 0;
			if (m_front_index == 0)
				find_index = index + 1;
			else
				find_index = (m_front_index + 1 + index) % m_maxSize;


			return m_ptr[find_index];
		}

		ComplexDeque& operator = (ComplexDeque& other)
		{
			if (m_ptr != nullptr)
				delete[] m_ptr;

			m_maxSize = other.m_maxSize;
			m_size = other.m_size;
			m_front_index = other.m_front_index;
			m_back_index = other.m_back_index;
			m_ptr = new T[m_maxSize];

			for (int i = 0; i < m_maxSize; i++)
			{
				m_ptr[i] = other.m_ptr[i];
			}
		}

	private:

		T* m_ptr;
		int m_maxSize;
		int m_size;

		int m_front_index;
		int m_back_index;

	};
}
