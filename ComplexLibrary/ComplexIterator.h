#pragma once

#include "ComplexNode.h"

namespace ComplexLibrary
{
	template <typename T>
	class ComplexIterator
	{
	public:

		ComplexIterator(ComplexNode<T>* p = nullptr)
			: m_cur(p)
		{

		}

		~ComplexIterator()
		{

		}

		ComplexIterator& operator ++ ()
		{
			if (m_cur == nullptr)
				throw ComplexNullptrException("iterator reference is null point.", "ComplexIterator", "operator ++");

			m_cur = m_cur->m_next;
			return *this;
		}

		ComplexIterator operator ++ (int)
		{
			ComplexIterator tmp = *this;
			++*this;
			return tmp;
		}

		ComplexNode<T>& operator * ()
		{
			return *m_cur;
		}

		ComplexNode<T>* operator -> ()
		{
			return m_cur;
		}

		bool operator == (const ComplexIterator& eq)
		{
			return m_cur == eq.m_cur;
		}

		bool operator != (const ComplexIterator& eq)
		{
			return m_cur != eq.m_cur;
		}

		bool operator == (const ComplexNode<T>* eq)
		{
			return m_cur == eq;
		}

		bool operator != (const ComplexNode<T>* eq)
		{
			return m_cur != eq;
		}

	private:

		ComplexNode<T>* m_cur;

	};
}

