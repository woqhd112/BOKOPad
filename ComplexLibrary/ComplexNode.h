#pragma once

#include "ComplexException.h"

namespace ComplexLibrary
{
	template <typename T> class ComplexNodeSet;

	template <typename T>
	class ComplexNode
	{
		template <typename T> friend class ComplexLinkedList;
		template <typename T> friend class ComplexIterator;
		template <typename T> friend class ComplexQueue;
		template <typename T> friend class ComplexStack;
		template <typename T> friend class ComplexNodeSet;
		template <typename T> friend class ComplexSet;
		template <typename T> friend class ComplexVector;

	public:

		T value;
		ComplexNode() {}
		~ComplexNode() {}
		ComplexNode(const T& value, ComplexNode<T>* ptr)
		{
			this->value = value;
			this->m_next = ptr;
		}

		bool has_next()
		{
			return (m_next != nullptr);
		}

		ComplexNode<T>* next()
		{
			if (this == nullptr)
				throw ComplexNullptrException("node is null point.", "ComplexNode", "next");

			return this->m_next;
		}

	private:

		ComplexNode<T>* m_next = nullptr;

	};


	template <typename T>
	class ComplexNodeSet
	{
		template <typename T> friend class ComplexSet;

	public:

		T value;
		ComplexNodeSet() : m_castingPtr(new ComplexNode<T>) { }
		~ComplexNodeSet() { delete m_castingPtr; }
		ComplexNodeSet(const T& value, ComplexNodeSet<T>* little_ptr, ComplexNodeSet<T>* too_ptr)
			: m_castingPtr(new ComplexNode<T>)
		{
			this->value = value;
			this->m_little = little_ptr;
			this->m_too = too_ptr;
		}


		bool operator < (ComplexNodeSet<T>& compare)
		{
			return (value < compare.value);
		}

	private:

		ComplexNodeSet<T>* m_parent = nullptr;
		ComplexNodeSet<T>* m_little = nullptr;
		ComplexNodeSet<T>* m_too = nullptr;
		ComplexNode<T>* m_castingPtr = nullptr;

		void SetValue(T value)
		{
			this->value = value;
			m_castingPtr->value = value;
		}
	};

}