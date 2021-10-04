#pragma once
#include <initializer_list>
#include "ComplexIterator.h"

// 벡터 이상하다..

namespace ComplexLibrary
{
	template <typename T>
	class ComplexVector
	{
	public:

		typedef ComplexIterator<T> iterator;

		ComplexVector(int capacity = 4)
			: m_head(nullptr)
			, m_ptr(nullptr)
			, m_size(0)
			, m_capacity(0)
		{
			m_ptr = new T[capacity];
			m_capacity = capacity;
			m_size = 0;
		}

		ComplexVector(ComplexVector<T>& ptr)
			: m_head(nullptr)
			, m_ptr(nullptr)
			, m_size(0)
			, m_capacity(0)
		{
			int nPtr_size = ptr.m_size;
			int nPtr_capacity = ptr.m_capacity;

			if (m_ptr != nullptr)
				delete[] m_ptr;

			m_ptr = new T[ptr.m_capacity];

			for (int i = 0; i < nPtr_capacity; i++)
			{
				m_ptr[i] = ptr.m_ptr[i];
			}

			m_size = nPtr_size;
			m_capacity = nPtr_capacity;

		}

		ComplexVector(std::initializer_list<T> list)
			: m_head(nullptr)
			, m_ptr(nullptr)
			, m_size(0)
			, m_capacity(0)
		{
			int size = list.size();
			m_size = size;
			m_capacity = size;

			if (m_ptr != nullptr)
				delete[] m_ptr;

			m_ptr = new T[size];

			auto iter = list.begin();

			int i = 0;
			while (iter != list.end())
			{
				m_ptr[i] = *iter;
				i++;
				iter++;
			}
		}

		~ComplexVector()
		{
			if (m_ptr != nullptr)
				delete[] m_ptr;

			if (m_head != nullptr)
			{
				ComplexNode<T>* cursor = m_head;
				while (cursor->has_next())
				{
					ComplexNode<T>* next = cursor->next();
					delete cursor;
					cursor = next;
				}
				delete cursor;
				m_head = nullptr;
			}
		}

		void push_back(T value)
		{
			if (m_size + 1 > m_capacity)
			{
				T* tmp = new T[m_capacity * 2];

				for (int i = 0; i < m_size; i++)
				{
					tmp[i] = m_ptr[i];
				}

				if (m_ptr != nullptr)
					delete[] m_ptr;

				m_ptr = tmp;
				m_capacity *= 2;

			}

			m_ptr[m_size] = value;

			ComplexNode<T>* newNode = new ComplexNode<T>;
			newNode->value = value;

			if (m_head == nullptr)
				m_head = newNode;
			else
			{
				ComplexNode<T>* cursor = m_head;
				while (cursor->has_next())
				{
					cursor = cursor->next();
				}
				cursor->m_next = newNode;
			}

			m_size++;
		}

		void reserve(int capacity)
		{
			if (capacity < m_capacity)
				return;

			T* tmp = new T[capacity];
			for (int i = 0; i < m_size; i++)
			{
				tmp[i] = m_ptr[i];
			}

			delete[] m_ptr;
			m_ptr = tmp;
			m_capacity = capacity;
		}

		T& at(int idx)
		{
			if (m_size <= 0)
				throw ComplexIndexOutOfBoundsException("vector size is zero.", "ComplexVector", "at");
			if ((m_size - 1) < idx)
				throw ComplexIndexOutOfBoundsException("vector call index is out of bounds.", "ComplexVector", "at");
			if (m_ptr == nullptr)
				throw ComplexNullptrException("vector reference is null point.", "ComplexVector", "at");

			return m_ptr[idx];
		}

		T& front() const
		{
			return m_ptr[0];
		}

		T& back() const
		{
			return m_ptr[m_size - 1];
		}

		iterator begin()
		{
			return iterator(m_head);
		}

		iterator end() const
		{
			return iterator(nullptr);
		}

		void erase(iterator iter)
		{
			if (m_size <= 0)
				throw ComplexIndexOutOfBoundsException("vector size is zero.", "ComplexVector", "erase");
			if (m_ptr == nullptr)
				throw ComplexNullptrException("vector reference is null point.", "ComplexVector", "erase");

			bool bFind = false;
			int i = 0;
			for (i = 0; i < m_size; i++)
			{
				if (m_ptr[i] == iter->value)
				{
					bFind = true;
					break;
				}
			}

			if (bFind)
				erase(i);
		}

		void erase(int idx)
		{
			if (m_size <= 0)
				throw ComplexIndexOutOfBoundsException("vector size is zero.", "ComplexVector", "erase");
			if ((m_size - 1) < idx)
				throw ComplexIndexOutOfBoundsException("vector call index is out of bounds.", "ComplexVector", "erase");
			if (m_ptr == nullptr)
				throw ComplexNullptrException("vector reference is null point.", "ComplexVector", "erase");

			delete_node(idx);

			T* tmp = new T[m_capacity];
			int tmpidx = 0;

			for (int i = 0; i < idx; i++)
			{
				tmp[tmpidx] = m_ptr[i];
				tmpidx++;
			}

			for (int i = idx + 1; i < m_size; i++)
			{
				tmp[tmpidx] = m_ptr[i];
				tmpidx++;
			}

			delete[] m_ptr;
			m_ptr = tmp;
			m_size--;
		}

		void erase(int startidx, int endidx)
		{
			if (m_size <= 0)
				throw ComplexIndexOutOfBoundsException("vector size is zero.", "ComplexVector", "erase");
			if ((m_size - 1) < startidx)
				throw ComplexIndexOutOfBoundsException("vector call start index is out of bounds.", "ComplexVector", "erase");
			if ((m_size - 1) < endidx)
				throw ComplexIndexOutOfBoundsException("vector call end index is out of bounds.", "ComplexVector", "erase");
			if (endidx < startidx)
				throw ComplexIndexOutOfBoundsException("vector call start index is more than call end index.", "ComplexVector", "erase");
			if (m_ptr == nullptr)
				throw ComplexNullptrException("vector reference is null point.", "ComplexVector", "at");

			T* tmp = new T[m_capacity];
			int tmpidx = 0;

			for (int i = 0; i < startidx; i++)
			{
				tmp[tmpidx] = m_ptr[i];
				tmpidx++;
			}

			for (int i = endidx + 1; i < m_size; i++)
			{
				tmp[tmpidx] = m_ptr[i];
				tmpidx++;
			}

			delete[] m_ptr;
			m_ptr = tmp;
			m_size = m_size - (endidx - startidx + 1);
		}

		void pop_back()
		{
			if (m_size <= 0)
			{
				m_size = 0;
			}
			else
			{
				m_size--;
			}
		}

		void clear()
		{
			m_size = 0;

			/*if (m_ptr != nullptr)
				delete[] m_ptr;*/

			if (m_head != nullptr)
			{
				while (m_head->has_next())
				{
					ComplexNode<T>* nextNode = m_head->next();
					delete m_head;
					m_head = nullptr;
					m_head = nextNode;
				}
				delete m_head;
				m_head = nullptr;
			}
		}

		int size() const
		{
			return m_size;
		}

		int capacity() const
		{
			return m_capacity;
		}

		bool empty() const
		{
			return m_size == 0;
		}

		ComplexVector& operator = (ComplexVector& other)
		{
			if (this != &other)
			{
				if (m_capacity != other.m_capacity)
				{
					m_capacity = other.m_capacity;
					delete[] m_ptr;

					m_ptr = new T[m_capacity];
				}

				m_size = other.m_size;
				for (int i = 0; i < m_size; i++)
				{
					m_ptr[i] = other.m_ptr[i];
				}
			}
			return *this;
		}

		ComplexVector& operator = (std::initializer_list<T> list)
		{
			int size = list.size();
			m_size = size;
			m_capacity = size;

			if (m_ptr != nullptr)
				delete[] m_ptr;

			m_ptr = new T[size];

			auto iter = list.begin();

			int i = 0;
			while (iter != list.end())
			{
				m_ptr[i] = *iter;
				i++;
				iter++;
			}

			return *this;
		}

		T& operator [] (int idx)
		{
			return m_ptr[idx];
		}

	private:

		T* m_ptr;
		ComplexNode<T>* m_head;
		int m_size;
		int m_capacity;

		void delete_node(int idx)
		{
			ComplexNode<T>* prev = nullptr;
			ComplexNode<T>* cursor = m_head;

			int i = 0;
			while (cursor->has_next())
			{
				if (i == idx)
					break;
				prev = cursor;
				cursor = cursor->next();
				i++;
			}

			// 커서가 맨 마지막까지 도달하였을 때
			if (!cursor->has_next())
			{
				//if (&value == &cursor->value)
				if (i == idx)
				{
					// 노드 개수가 1개 초과일 때
					if (prev != nullptr)
					{
						prev->m_next = cursor->m_next;
					}
					// 노드 개수가 1개일 때 (초반과 끝이 같을 때)
					else
					{
						m_head = nullptr;
					}
				}
			}
			// 커서가 끝지점이 아닐 때
			else
			{
				// 노드가 두번 째 이상일 때
				if (prev != nullptr)
				{
					prev->m_next = cursor->m_next;
				}
				// 노드가 첫번 째일 떄
				else
				{
					m_head = cursor->m_next;
				}
			}
			delete cursor;
			cursor = nullptr;
		}
	};
}

