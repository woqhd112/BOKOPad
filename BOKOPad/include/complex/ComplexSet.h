#pragma once

#include <initializer_list>
#include "ComplexIterator.h"

namespace ComplexLibrary
{
	// 2진트리로 구현
	// set 사용시 insert 하는 데이터가 구조체 혹은 클래스이면 < 연산자 오버로딩 구현할 것
	template <typename T>
	class ComplexSet
	{
	public:

		typedef ComplexIterator<T> iterator;

		ComplexSet()
			: m_size(0)
			, m_root(nullptr)
			, m_sortValue(nullptr)
			, m_pos(m_root)
		{

		}

		ComplexSet(ComplexSet<T>& ptr)
			: m_size(0)
			, m_root(nullptr)
			, m_sortValue(nullptr)
			, m_pos(m_root)
		{
			clear();
			iterator iter = ptr.begin();
			while (iter != ptr.end())
			{
				insert(iter->value);
				iter++;
			}
		}

		ComplexSet(ComplexSet<T>&& ptr)
			: m_size(0)
			, m_root(nullptr)
			, m_sortValue(nullptr)
			, m_pos(m_root)
		{
			clear();
			iterator iter = ptr.begin();
			while (iter != ptr.end())
			{
				insert(iter->value);
				iter++;
			}
		}

		ComplexSet(std::initializer_list<T> list)
			: m_size(0)
			, m_root(nullptr)
			, m_sortValue(nullptr)
			, m_pos(m_root)
		{
			clear();
			auto iter = list.begin();
			while (iter != list.end())
			{
				insert(*iter);
				iter++;
			}
		}

		~ComplexSet()
		{
			clear();
		}

		// 같은 값이 들어오면 아무 처리하지 않는다. 오류도없음.
		void insert(T value)
		{
			ComplexNodeSet<T>* newNode = new ComplexNodeSet<T>;
			newNode->SetValue(value);

			if (m_root == nullptr)
			{
				m_root = newNode;
				m_size++;
				m_sortValue = m_root->m_castingPtr;
				return;
			}

			if (insert_value(m_root, newNode, value) == false)
				delete newNode;
		}

		void erase(T value)
		{
			if (find_value(m_root, value) == false)
				throw ComplexNotFoundException("set call key is not found.", "ComplexSet", "erase");

			// 삭제는 메인 value가 지워지면 too가 그자리를 대체
			// 크기는 too > 메인 > little 순이므로
			// little은 too의 하위 노드들의 가장 작은 little의 little로 붙어야함

			delete_node(value);

			ComplexNodeSet<T>* parent = m_pos->m_parent;
			m_pos = m_root;

			// 값이 m_root 일때임
			if (parent == nullptr)
			{
				ComplexNodeSet<T>* little = m_pos->m_little;
				ComplexNodeSet<T>* too = m_pos->m_too;

				delete m_pos;
				m_pos = nullptr;

				if (little == nullptr && too == nullptr)
				{
					// 이조건은 올 수 없다.
				}
				else if (little != nullptr && too == nullptr)
				{
					m_root = little;
				}
				else if (little == nullptr && too != nullptr)
				{
					m_root = too;
				}
				else
				{
					m_root = too;
					// child_little 은 child_too의 가장 낮은 little을 찾아 그 포인터의 little에 붙힌다.

					T inputValue = too->value;
					find_min(too, inputValue);
					if (find_value(m_root, inputValue))
					{
						little->m_parent = m_root;
						m_pos->m_little = little;
					}
				}
				m_root->m_parent = nullptr;
				return;
			}


			bool bCheckChildLittle = false;
			if (parent->m_little == nullptr && parent->m_too == nullptr)
				throw ComplexNotFoundException("set call key is not found.", "ComplexSet", "erase");
			else if (parent->m_little != nullptr)
			{
				if (parent->m_little->value == value)
					bCheckChildLittle = true;
			}

			ComplexNodeSet<T>* child = bCheckChildLittle ? parent->m_little : parent->m_too;
			ComplexNodeSet<T>* child_little = child->m_little;
			ComplexNodeSet<T>* child_too = child->m_too;

			delete child;
			child = nullptr;
			m_size--;

			if (bCheckChildLittle)
			{
				if (child_little == nullptr && child_too == nullptr)
				{
					parent->m_little = nullptr;
				}
				else if (child_little != nullptr && child_too == nullptr)
				{
					child_little->m_parent = parent;
					parent->m_little = child_little;
				}
				else if (child_little == nullptr && child_too != nullptr)
				{
					child_too->m_parent = parent;
					parent->m_little = child_too;
				}
				else
				{
					child_too->m_parent = parent;
					parent->m_little = child_too;
					// child_little 은 child_too의 가장 낮은 little을 찾아 그 포인터의 little에 붙힌다.

					T inputValue = child_too->value;
					find_min(child_too, inputValue);
					if (find_value(m_root, inputValue))
					{
						child_little->m_parent = parent;
						m_pos->m_little = child_little;
					}
				}
			}
			else
			{
				if (child_little == nullptr && child_too == nullptr)
				{
					parent->m_too = nullptr;
				}
				else if (child_little != nullptr && child_too == nullptr)
				{
					child_little->m_parent = parent;
					parent->m_too = child_little;
				}
				else if (child_little == nullptr && child_too != nullptr)
				{
					child_too->m_parent = parent;
					parent->m_too = child_too;
				}
				else
				{
					child_too->m_parent = parent;
					parent->m_too = child_too;
					// child_little 은 child_too의 가장 낮은 little을 찾아 그 포인터의 little에 붙힌다.

					T inputValue = child_too->value;
					find_min(child_too, inputValue);
					if (find_value(m_root, inputValue))
					{
						child_little->m_parent = parent;
						m_pos->m_little = child_little;
					}
				}
			}
		}

		void erase(iterator iter)
		{
			erase(iter->value);
		}

		void erase(iterator begin, iterator end)
		{
			if (begin == nullptr)
				return;
			else if (begin != nullptr && end == nullptr)
			{
				iterator begin_iter = begin;
				iterator end_iter = end;
				while (begin_iter != end_iter)
				{
					erase(begin_iter++);
				}
			}
			else if (begin != nullptr && end != nullptr)
			{
				iterator begin_iter = begin;
				iterator end_iter = end + 1;
				while (begin_iter != end_iter)
				{
					erase(begin_iter++);
				}
			}
		}

		iterator find(T value)
		{
			if (find_value(m_root, value) == false)
				return end();

			iterator iter = begin();

			while (iter != end())
			{
				T t = iter->value;
				if (iter->value < value) {}
				else if (value < iter->value) {}
				else
					break;
				iter++;
			}

			return iter;
		}

		iterator begin()
		{
			return iterator(m_sortValue);
		}

		iterator end()
		{
			return iterator(nullptr);
		}

		bool empty()
		{
			return (m_size == 0);
		}

		int size() const
		{
			return m_size;
		}

		void clear()
		{
			iterator iter = begin();

			while (iter != end())
			{
				erase(iter++);
			}

			m_root = nullptr;
			m_pos = nullptr;
			m_sortValue = nullptr;
			m_size = 0;
		}

		int count(T value)
		{
			iterator iter = begin();

			int set_count = 0;
			while (iter != end())
			{
				T t = iter->value;
				if (iter->value < value) {}
				else if (value < iter->value) {}
				else
					set_count++;
				iter++;
			}

			return set_count;
		}

		ComplexSet<T>& operator = (ComplexSet<T>& other)
		{
			clear();
			m_size = other.m_size;
			m_root = other.m_root;
			m_sortValue = other.m_sortValue;

			iterator iter = other.begin();
			while (iter != other.end())
			{
				insert(iter->value);
				iter++;
			}
			return *this;
		}

		ComplexSet<T>& operator = (std::initializer_list<T> list)
		{
			clear();
			auto iter = list.begin();
			while (iter != list.end())
			{
				insert(*iter);
				iter++;
			}
			return *this;
		}

	private:

		bool find_max(ComplexNodeSet<T>* ptr, T& value)
		{
			bool bReturn = false;
			if (ptr->m_too == nullptr)
				return false;
			else if (ptr->m_too != nullptr)
			{
				if (value < ptr->too->value)	// 사실 이 조건은 안해도 될거같다..
				{
					if (find_max(ptr->m_too, value) == false)
					{
						value = ptr->m_too->value;
						bReturn = true;
					}
				}
			}

			return bReturn;
		}

		bool find_min(ComplexNodeSet<T>* ptr, T& value)
		{
			bool bReturn = false;
			if (ptr->m_little == nullptr)
				return false;
			else if (ptr->m_little != nullptr)
			{
				if (ptr->m_little->value < value)	// 사실 이 조건은 안해도 될거같다..
				{
					if (find_min(ptr->m_little, value) == false)
					{
						value = ptr->m_little->value;
						bReturn = true;
					}
				}
			}

			return bReturn;
		}

		bool insert_value(ComplexNodeSet<T>* ptr, ComplexNodeSet<T>* newNode, T& value)
		{
			if (ptr == nullptr)
				return false;

			if (ptr->value < value)
			{
				insert_too_value(ptr, newNode, value);
			}
			else if (value < ptr->value)
			{
				insert_little_value(ptr, newNode, value);
			}
			else
				return false;

			return true;
		}

		void insert_too_value(ComplexNodeSet<T>* ptr, ComplexNodeSet<T>* newNode, T& value)
		{
			bool bTooNull = false;
			if (ptr->m_too == nullptr)
				bTooNull = true;

			if (bTooNull)
			{
				insert_node(newNode, value);

				newNode->m_parent = ptr;
				ptr->m_too = newNode;
				m_size++;
			}
			else
			{
				insert_value(ptr->m_too, newNode, value);
			}
		}

		void insert_little_value(ComplexNodeSet<T>* ptr, ComplexNodeSet<T>* newNode, T& value)
		{
			bool bLittleNull = false;
			if (ptr->m_little == nullptr)
				bLittleNull = true;

			if (bLittleNull)
			{
				insert_node(newNode, value);

				newNode->m_parent = ptr;
				ptr->m_little = newNode;
				m_size++;
			}
			else
			{
				insert_value(ptr->m_little, newNode, value);
			}
		}

		void delete_node(T& value)
		{
			ComplexNode<T>* prev = nullptr;
			ComplexNode<T>* cursor = m_sortValue;

			while (cursor->has_next())
			{
				if (value < cursor->value) {}
				else if (cursor->value < value) {}
				else
					break;
				prev = cursor;
				cursor = cursor->next();
			}

			// 커서가 맨 마지막까지 도달하였을 때
			if (!cursor->has_next())
			{
				if (value < cursor->value) {}
				else if (cursor->value < value) {}
				else
				{
					// 노드 개수가 1개 초과일 때
					if (prev != nullptr)
					{
						prev->m_next = cursor->m_next;
					}
					// 노드 개수가 1개일 때 (초반과 끝이 같을 때)
					else
					{
						m_sortValue = nullptr;
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
					m_sortValue = cursor->m_next;
				}
			}
		}

		void insert_node(ComplexNodeSet<T>* newNode, T& value)
		{
			ComplexNode<T>* prev = nullptr;
			ComplexNode<T>* cursor = m_sortValue;

			while (cursor->has_next())
			{
				if (value < cursor->value)
					break;
				prev = cursor;
				cursor = cursor->next();
			}

			if (!cursor->has_next())
			{
				if (value < cursor->value)
				{
					if (prev != nullptr)
					{
						prev->m_next = newNode->m_castingPtr;
						newNode->m_castingPtr->m_next = cursor;
					}
					else
					{
						newNode->m_castingPtr->m_next = cursor->m_next;
						cursor->m_next = newNode->m_castingPtr;
					}
				}
				else if (cursor->value < value)
				{
					newNode->m_castingPtr->m_next = cursor->m_next;
					cursor->m_next = newNode->m_castingPtr;
				}
			}
			else
			{
				if (prev != nullptr)
				{
					prev->m_next = newNode->m_castingPtr;
					newNode->m_castingPtr->m_next = cursor;
				}
				else
				{
					newNode->m_castingPtr->m_next = m_sortValue;
					m_sortValue = newNode->m_castingPtr;
				}
			}
		}

		bool find_value(ComplexNodeSet<T>* ptr, T& value)
		{
			if (ptr == nullptr)
				return false;

			bool bReturn = false;

			if (ptr->value < value)
			{
				bReturn = find_value(ptr->m_too, value);
			}
			else if (value < ptr->value)
			{
				bReturn = find_value(ptr->m_little, value);
			}
			else
			{
				// find!!
				m_pos = ptr;
				bReturn = true;
			}
			return bReturn;
		}

		ComplexNodeSet<T>* m_root;
		ComplexNodeSet<T>* m_pos;

		ComplexNode<T>* m_sortValue;

		int m_size;

	};
}