#pragma once

#include <initializer_list>
#include "ComplexIterator.h"

namespace ComplexLibrary
{
	template <typename T>
	class ComplexLinkedList
	{
	private:
		ComplexNode<T>* m_head;
		ComplexNode<T>* m_tail;
		int m_size;

	public:

		typedef ComplexIterator<T> iterator;

		ComplexLinkedList()
			: m_head(nullptr), m_tail(nullptr), m_size(0) {}

		ComplexLinkedList(ComplexLinkedList<T>& ptr)
			: m_head(nullptr), m_tail(nullptr), m_size(0)
		{
			clear();
			iterator iter = ptr.begin();
			while (iter != ptr.end())
			{
				push_tail(iter->value);
				iter++;
			}
		}

		ComplexLinkedList(std::initializer_list<T> list)
			: m_head(nullptr), m_tail(nullptr), m_size(0)
		{
			clear();
			auto iter = list.begin();
			while (iter != list.end())
			{
				push_tail(*iter);
				iter++;
			}
		}

		~ComplexLinkedList() { clear(); }

		void push_tail(T value)
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
				m_tail->m_next = node;
				m_tail = m_tail->m_next;
			}
		}

		void push_head(T value)
		{
			ComplexNode<T>* node = new ComplexNode<T>;
			m_size++;

			if (m_head != nullptr)
			{
				node->m_next = m_head;
			}
			node->value = value;
			m_head = node;
		}

		void push(int index, T value)
		{
			if (m_size < index || index < 0)
				throw ComplexIndexOutOfBoundsException("linked list call index is out of bounds.", "ComplexLinkedList", "push");

			if (m_head == nullptr || m_size == index)
			{
				push_tail(value);
				return;
			}

			ComplexNode<T>* moveNode = m_head;
			ComplexNode<T>* containNode = moveNode;
			ComplexNode<T>* node = new ComplexNode<T>;

			node->value = value;
			node->m_next = nullptr;

			int cnt = 0;
			while (moveNode != nullptr)
			{
				if (cnt == index)
				{
					containNode = moveNode;
					moveNode = node;
					moveNode->m_next = containNode;
					break;
				}
				moveNode = moveNode->m_next;
				cnt++;
			}

			m_size++;
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

		void erase(int index)
		{
			if (m_size <= index || index < 0)
				throw ComplexIndexOutOfBoundsException("linked list call index is out of bounds.", "ComplexLinkedList", "erase");
			if (m_head == nullptr)
				throw ComplexNullptrException("linked list head node is null point.", "ComplexLinkedList", "erase");


			ComplexNode<T>* cursor = m_head;
			ComplexNode<T>* prevNode = nullptr;

			int cnt = 0;
			while (cursor != nullptr)
			{
				if (cnt == index)
					break;

				prevNode = cursor;
				cursor = cursor->next();
				cnt++;
			}

			
			// 인덱스가 0번일 때 (사이즈가 0일때는 위에서 예외처리함)
			if (prevNode == nullptr)
			{
				if (m_size > 1)
				{
					--m_size;
					m_head = cursor->next();
					delete cursor;
					cursor = nullptr;
				}
				else
				{
					m_size = 0;
					delete m_head;
					m_head = nullptr;
					m_tail = nullptr;
				}
			}
			else
			{
				m_size--;
				prevNode->m_next = cursor->m_next;
				delete cursor;
				cursor = nullptr;
			}
		}

		ComplexNode<T>* get_at(int index)
		{
			if (m_size <= index || index < 0)
				throw ComplexIndexOutOfBoundsException("linked list call index is out of bounds.", "ComplexLinkedList", "get_at");
			if (m_head == nullptr)
				throw ComplexNullptrException("linked list head node is null point.", "ComplexLinkedList", "get_at");

			ComplexNode<T>* findNode = m_head;

			int cnt = 0;
			while (findNode != nullptr)
			{
				if (cnt == index)
				{
					break;
				}
				findNode = findNode->m_next;
				cnt++;
			}

			return findNode;
		}

		ComplexNode<T>* get_head()
		{
			return m_head;
		}

		ComplexNode<T>* get_tail()
		{
			return m_tail;
		}

		T& at(int index)
		{
			if (m_size <= index || index < 0)
				throw ComplexIndexOutOfBoundsException("linked list call index is out of bounds.", "ComplexLinkedList", "at");
			if (m_head == nullptr)
				throw ComplexNullptrException("linked list head node is null point.", "ComplexLinkedList", "at");

			ComplexNode<T>* findNode = m_head;

			int cnt = 0;
			while (findNode != nullptr)
			{
				if (cnt == index)
				{
					break;
				}
				findNode = findNode->m_next;
				cnt++;
			}

			return findNode->value;
		}

		iterator begin()
		{
			return iterator(m_head);
		}

		iterator end()
		{
			return iterator(nullptr);
		}

		T& head()
		{
			return m_head->value;
		}

		T& tail()
		{
			return m_tail->value;
		}

		bool empty()
		{
			return (m_size <= 0);
		}

		int size()
		{
			return m_size;
		}

		ComplexLinkedList<T>& operator = (ComplexLinkedList<T>& other)
		{
			clear();
			iterator iter = other.begin();
			while (iter != other.end())
			{
				push_tail(iter->value);
				iter++;
			}
			return *this;
		}

		ComplexLinkedList<T>& operator = (std::initializer_list<T>& list)
		{
			clear();
			auto iter = list.begin();
			while (iter != list.end())
			{
				push_tail(*iter);
				iter++;
			}
			return *this;
		}

	};
}