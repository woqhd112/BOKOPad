#pragma once

#include "ComplexLinkedList.h"
#include "ComplexMap.h"
#include "ComplexQueue.h"
#include "ComplexSet.h"
#include "ComplexStack.h"
#include "ComplexVector.h"
#include "ComplexSingleton.h"
#include "ComplexDeque.h"

namespace ComplexLibrary
{
	class ComplexCollection
	{
	public:

		ComplexCollection()
		{
		
		}

		virtual ~ComplexCollection()
		{
		
		}

		template <typename T>
		ComplexVector<T>* GetVectorInstance()
		{
			return ComplexSingleton<ComplexVector<T>>::GetInstancePtr();
		}

		template <typename T>
		void ClearVectorInstance()
		{
			ComplexSingleton<ComplexVector<T>>::DestroyInstance();
		}

		template <typename T>
		ComplexQueue<T>* GetQueueInstance()
		{
			return ComplexSingleton<ComplexQueue<T>>::GetInstancePtr();
		}

		template <typename T>
		void ClearQueueInstance()
		{
			ComplexSingleton<ComplexQueue<T>>::DestroyInstance();
		}

		template <typename T>
		ComplexStack<T>* GetStackInstance()
		{
			return ComplexSingleton<ComplexStack<T>>::GetInstancePtr();
		}

		template <typename T>
		void ClearStackInstance()
		{
			ComplexSingleton<ComplexStack<T>>::DestroyInstance();
		}

		template <typename T>
		ComplexSet<T>* GetSetInstance()
		{
			return ComplexSingleton<ComplexSet<T>>::GetInstancePtr();
		}

		template <typename T>
		void ClearSetInstance()
		{
			ComplexSingleton<ComplexSet<T>>::DestroyInstance();
		}

		template <typename T, typename N>
		ComplexMap<T, N>* GetMapInstance()
		{
			return ComplexSingleton<ComplexMap<T, N>>::GetInstancePtr();
		}

		template <typename T, typename N>
		void ClearMapInstance()
		{
			ComplexSingleton<ComplexMap<T, N>>::DestroyInstance();
		}

		template <typename T>
		ComplexLinkedList<T>* GetListInstance()
		{
			return ComplexSingleton<ComplexLinkedList<T>>::GetInstancePtr();
		}

		template <typename T>
		void ClearListInstance()
		{
			ComplexSingleton<ComplexLinkedList<T>>::DestroyInstance();
		}

		template <typename T>
		ComplexDeque<T>* GetDequeInstance()
		{
			return ComplexSingleton<ComplexDeque<T>>::GetInstancePtr();
		}

		template <typename T>
		void ClearDequeInstance()
		{
			ComplexSingleton<ComplexDeque<T>>::DestroyInstance();
		}

	private:


	};
}