#pragma once

#include <initializer_list>
#include "ComplexNode.h"

namespace ComplexLibrary
{
	template <typename T>
	class ComplexStack
	{
	public:

		ComplexStack()
			: m_head(nullptr), m_tail(nullptr), m_size(0)
		{

		}

		ComplexStack(ComplexStack<T>& ptr)
			: m_head(nullptr), m_tail(nullptr), m_size(0)
		{
			clear();
			if (ptr.m_head != nullptr)
			{
				ComplexNode<T>* cursor = ptr.m_head;
				while (cursor->has_next())
				{
					push(cursor->value);
					cursor = cursor->next();
				}
				push(cursor->value);
			}
		}

		ComplexStack(std::initializer_list<T> list)
			: m_head(nullptr), m_tail(nullptr), m_size(0)
		{
			clear();
			auto iter = list.begin();
			while (iter != list.end())
			{
				push(*iter);
				iter++;
			}
		}

		~ComplexStack()
		{

		}

		void push(T value)
		{
			ComplexNode<T>* node = new ComplexNode<T>;
			m_size++;

			node->value = value;
			node->m_next = nullptr;

			if (m_head == nullptr)
			{
				m_head = node;
				m_tail = node;
			}
			else
			{
				node->m_next = m_head;
				m_head = node;
			}
		}

		T& top() const
		{
			return m_head->value;
		}

		void pop()
		{
			if (m_head == nullptr)
				throw ComplexNullptrException("queue head node is null point.", "ComplexStack", "pop");

			ComplexNode<T>* old = m_head;

			m_head = m_head->m_next;
			delete old;
			old = nullptr;

			m_size--;

			if (m_size <= 0)
			{
				m_head = nullptr;
				m_tail = nullptr;
				m_size = 0;
			}
		}

		int size()
		{
			return m_size;
		}

		bool empty()
		{
			return (m_size <= 0);
		}

		void clear()
		{
			ComplexNode<T>* deleteNode = m_head;

			while (deleteNode != nullptr)
			{
				m_head = deleteNode->m_next;
				delete deleteNode;
				deleteNode = m_head;
			}
			m_head = nullptr;
			m_tail = nullptr;
			m_size = 0;
		}

		ComplexStack<T>& operator = (ComplexStack<T>& other)
		{
			clear();
			if (other.m_head != nullptr)
			{
				ComplexNode<T>* cursor = other.m_head;
				while (cursor->has_next())
				{
					push(cursor->value);
					cursor = cursor->next();
				}
				push(cursor->value);
			}

			return *this;
		}


		ComplexStack<T>& operator = (std::initializer_list<T>& list)
		{
			clear();
			auto iter = list.begin();
			while (iter != list.end())
			{
				push(*iter);
				iter++;
			}

			return *this;
		}

	private:

		ComplexNode<T>* m_head;
		ComplexNode<T>* m_tail;
		int m_size;

	};
}