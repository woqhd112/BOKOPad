#pragma once

#include <condition_variable>
#include "ComplexLock.h"

namespace ComplexLibrary
{
	// unique_lock °ú È£È¯
	class ComplexCondition
	{
	public:

		ComplexCondition()
		{

		}

		virtual ~ComplexCondition()
		{

		}

		void Destroy()
		{
			Signal();
		}

		void Signal()
		{
			ComplexScopedLock lock(&m_lock);
			m_cond.notify_all();
		}

		void Wait()
		{
			ComplexScopedLock lock(&m_lock);
			m_cond.wait(m_lock.m_uniquelock);
		}

		// return : false -> time out
		bool WaitFor(ComplexLock& lock, int second)
		{
			std::cv_status status = m_cond.wait_for(lock.m_uniquelock, std::chrono::seconds(second));
			if (status == std::cv_status::timeout)
				return false;
			return true;
		}

	private:

		ComplexLock m_lock;
		std::condition_variable m_cond;
	};
}