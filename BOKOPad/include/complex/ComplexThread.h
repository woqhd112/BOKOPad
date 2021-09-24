#pragma once

#include "ComplexLock.h"

namespace ComplexLibrary
{

	class ComplexThread
	{
	public:

		enum ThreadStatus
		{
			THREAD_STOP = 0,
			THREAD_RUNNING
		};

		ComplexThread()
			: m_sleep(false)
		{
			m_threadStatus = THREAD_STOP;
		}

		virtual ~ComplexThread()
		{

		}

		static unsigned __stdcall MainThread(void* param)
		{
			ComplexThread* pThis = (ComplexThread*)param;
			pThis->Run();

			return 0;
		}

		bool Start()
		{
			//ComplexUniqueLock lock(&m_lock);

			if (m_threadStatus == THREAD_RUNNING)
				return false;

			m_threadStatus = THREAD_RUNNING;
			m_thread = std::thread(MainThread, this);

			return true;
		}

		bool Stop()
		{
			//ComplexUniqueLock lock(&m_lock);

			if (m_threadStatus == THREAD_STOP)
				return false;

			m_threadStatus = THREAD_STOP;
			return true;
		}

		void Join()
		{
			//ComplexUniqueLock lock(&m_lock);

			if (m_thread.joinable())
			{
				m_threadStatus = THREAD_STOP;
				m_thread.join();
			}
		}

		bool IsRunning()
		{
			//ComplexUniqueLock lock(&m_lock);

			return m_threadStatus == THREAD_RUNNING;
		}

		static void Sleep(int millisecond)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(millisecond));
		}

		static void YieldThread()
		{
			std::this_thread::yield();
		}

		virtual void Run() = 0;

	private:

		ThreadStatus m_threadStatus;
		std::thread m_thread;
		//ComplexLock m_lock;

		bool m_sleep;

	};
}