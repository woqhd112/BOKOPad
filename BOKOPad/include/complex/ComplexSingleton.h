#pragma once

namespace ComplexLibrary
{
	template <typename T>
	class ComplexSingleton
	{
	protected:
		ComplexSingleton() {}
		virtual ~ComplexSingleton() {}

	public:
		static T& GetInstance()
		{
			return *CreateInstance();
		}

		static T* GetInstancePtr()
		{
			return CreateInstance();
		}

		static void DestroyInstance()
		{
			if (m_pInstance != NULL)
			{
				delete m_pInstance;
				m_pInstance = NULL;
			}
		}

	private:

		static T* CreateInstance()
		{
			T* tmp = m_pInstance;

			if (tmp == NULL)
			{
				tmp = new T;
				m_pInstance = tmp;
			}

			return tmp;
		}


		static T* m_pInstance;

	};

	template<typename T> T* ComplexSingleton<T>::m_pInstance = NULL;
}