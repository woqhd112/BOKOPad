#pragma once

#include <mutex>
#include <thread>
#include <chrono>

namespace ComplexLibrary
{
	class ComplexLock
	{
		friend class ComplexCondition;
	public:

		ComplexLock(ComplexLock* lock = nullptr)
		{
			AttachCriticalSection(lock);
		}

		virtual ~ComplexLock()
		{
			DetachCriticalSection();
		}

		void AttachCriticalSection(ComplexLock* lock = nullptr)
		{
			if (lock == nullptr)
				m_uniquelock = std::unique_lock<std::mutex>(m_mutex, std::defer_lock);
			else
				m_uniquelock = std::unique_lock<std::mutex>(lock->m_mutex, std::defer_lock);
		}

		void DetachCriticalSection()
		{
			m_uniquelock.release();
		}

		void Lock()
		{
			try
			{
				m_uniquelock.lock();
			}
			catch (std::system_error e)
			{
				// 데드락인데 lock이 점유 성공할 때 까지 blocking 시킴
				Lock();
			}
		}

		void UnLock()
		{
			try
			{
				m_uniquelock.unlock();
			}
			catch (std::system_error e)
			{
				// 이미 unlock 상태이거나 lock이 초기화안된 상태 그냥 리턴시킴
				return;
			}
		}

		bool TryLock()
		{
			bool btry = false;
			try
			{
				btry = m_uniquelock.try_lock();
			}
			catch (std::system_error e)
			{
				// 데드락 발생
				btry = false;
			}
			return btry;
		}

	private:

		std::mutex m_mutex;
		std::unique_lock<std::mutex> m_uniquelock;
	};

	class ComplexScopedLock
	{
		friend class ComplexCondition;
	public:

		explicit ComplexScopedLock(ComplexLock* lock)
		{
			m_lock.AttachCriticalSection(lock);
			m_lock.Lock();
		}

		virtual ~ComplexScopedLock()
		{
			m_lock.UnLock();
		}

		void Lock()
		{
			m_lock.Lock();
		}

		void UnLock()
		{
			m_lock.UnLock();
		}

		ComplexLock& GetLock()
		{
			return m_lock;
		}

	private:

		ComplexLock m_lock;
	};
}