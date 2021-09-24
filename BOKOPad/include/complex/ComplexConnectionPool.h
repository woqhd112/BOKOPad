#pragma once

#include "ComplexDatabase.h"
#include "ComplexPool.h"
#include "ComplexThread.h"
#include "ComplexCondition.h"
#include "ComplexUniqueQueue.h"

namespace ComplexLibrary
{
	class ComplexConnectionPoolTimer : public ComplexThread
	{
	public:

		ComplexConnectionPoolTimer()
			: m_timeout(0)
		{
		}

		virtual ~ComplexConnectionPoolTimer()
		{
			StopTimer();
		}

		void StartTimer()
		{
			if (this->IsRunning() == false)
				this->Start();
		}

		void StopTimer()
		{
			if (this->IsRunning())
			{
				this->Stop();
				this->Join();
				m_timeout = 0;
			}
		}

	protected:

		virtual void Run()
		{
			while (this->IsRunning())
			{
				ComplexThread::Sleep(1000);
				m_timeout++;
			}
		}

	private:

		int m_timeout;

	};

	class ComplexConnectionPool : public ComplexPool<ComplexDatabase>, public ComplexThread
	{
	public:

		ComplexConnectionPool(int poolCapacity = 10)
			//: m_connectionTimer(new ComplexConnectionPoolTimer)	// 사용 안함
		{
			SetCapacity(poolCapacity);
			//m_connectionTimer->StartTimer();	// 사용 안함
			this->Start();
		}

		~ComplexConnectionPool()
		{
			/*if (m_connectionTimer)
			{
				delete m_connectionTimer;
				m_connectionTimer = nullptr;
			}*/	// 사용 안함
			this->Stop();
			m_cond.Destroy();
			this->Join();
		}

		bool InitializeDBFile(ComplexString filePath, ComplexString* error = nullptr)
		{
			ComplexVector<ComplexPoolData<ComplexDatabase>*>* poolDataFactory = GetPoolDataFactory();

			bool bSuccess = false;
			for (int i = 0; i < poolDataFactory->size(); i++)
			{
				ComplexPoolData<ComplexDatabase>* data = poolDataFactory->at(i);
				ComplexDatabase* dbInstance = data->GetInstance();
				if (error == nullptr)
					bSuccess = dbInstance->ConnectDatabase(filePath);
				else
					bSuccess = dbInstance->ConnectDatabase(filePath, error);
				if (!bSuccess)
					return false;
			}
			return true;
		}

		ComplexString Connect()
		{
			ComplexString poolID = "";
			ComplexPoolData<ComplexDatabase>* poolData;

			poolID = ConnectPool();
			poolData = GetPoolData(poolID);
			int time_out_cnt = 0;
			while (poolData == nullptr)
			{
				ComplexThread::Sleep(1000);
				time_out_cnt++;
				poolID = ConnectPool();
				poolData = GetPoolData(poolID);

				if (time_out_cnt == 5)
					break;
			}

			if (poolData == nullptr)
				return "";

			m_connectionQueue.lock();
			m_connectionQueue.push(poolData);
			m_connectionQueue.unlock();
			m_cond.Signal();
			ComplexThread::YieldThread();

			return poolID;
		}

		bool DisConnect(ComplexString poolID)
		{
			bool suc = DisConnectPool(poolID);

			if (suc)
			{
				m_connectionQueue.lock();
				if (!m_connectionQueue.empty())
				{
					m_connectionQueue.pop();
				}
				m_connectionQueue.unlock();
			}

			return suc;
		}

		ComplexDatabase* GetConnectInstance(ComplexString poolID)
		{
			return GetInstance(poolID);
		}

		virtual void Run()
		{
			ComplexPoolData<ComplexDatabase>* poolData;

			while (this->IsRunning())
			{
				m_cond.Wait();

				m_connectionQueue.lock();
				if (m_connectionQueue.empty())
				{
					m_connectionQueue.unlock();
					ComplexThread::YieldThread();
					continue;
				}
				poolData = m_connectionQueue.front();
				m_connectionQueue.unlock();

				// 연결 되었으니 밑작업

			}
		}

	private:


		void ReserveConnectionSize(int size)
		{
			ReservePoolSize(size);
		}

	private:

		//ComplexConnectionPoolTimer* m_connectionTimer;	// 사용 안함
		ComplexCondition m_cond;
		ComplexUniqueQueue<ComplexPoolData<ComplexDatabase>*> m_connectionQueue;

	};
}