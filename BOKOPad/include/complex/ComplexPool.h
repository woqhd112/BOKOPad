#pragma once

#include "ComplexVector.h"
#include "ComplexConvert.h"

namespace ComplexLibrary
{
	template <typename T>
	class ComplexPoolData
	{
	public:

		ComplexPoolData()
			: m_isConnect(false)
		{
			m_ptr = new T;
		}

		virtual ~ComplexPoolData()
		{
			delete m_ptr;
			m_ptr = nullptr;
		}

		bool IsConnect() const
		{
			return m_isConnect;
		}

		void SetConnect(bool con)
		{
			m_isConnect = con;
		}

		T* GetInstance() const
		{
			return m_ptr;
		}

	private:

		T* m_ptr;
		bool m_isConnect;

	};

#define MAX_POOL_CAPACITY			999
#define MAX_POOL_CAPACITY_FORMAT	"%03d"
#define MAX_POOL_CAPACITY_LENGTH	6

	template <typename T>
	class ComplexPool
	{
		typedef ComplexVector<ComplexPoolData<T>*> PoolDataFactory;
	public:

		ComplexPool(int poolCapacity = 10)
			: m_poolCapacity(poolCapacity)
			, m_useConnectionCount(0)
		{
			if (poolCapacity > MAX_POOL_CAPACITY)
				throw ComplexCapacityOverflowException("pool insert capacity is maxsize.", "ComplexPool", "constructor");
			else if (poolCapacity < 1)
				throw ComplexIndexOutOfBoundsException("pool insert capacity is at least 1.", "ComplexPool", "constructor");

			for (int i = 1; i <= poolCapacity; i++)
			{
				ComplexPoolData<T>* newPoolData = new ComplexPoolData<T>;
				m_pool.push_back(newPoolData);
			}
		}

		virtual ~ComplexPool()
		{
			for (int i = 0; i < m_pool.size(); i++)
			{
				ComplexPoolData<T>* poolData = m_pool.at(i);
				delete poolData;
				poolData = nullptr;
			}
			m_pool.clear();
		}

	protected:

		void SetCapacity(int capacity)
		{
			m_poolCapacity = capacity;
		}

		int GetCapacity() const
		{
			return m_poolCapacity;
		}

		ComplexString ConnectPool()
		{
			int i = 0;
			ComplexPoolData<T>* poolData = nullptr;
			for (i = 0; i < m_pool.size(); i++)
			{
				poolData = m_pool.at(i);
				if (poolData->IsConnect() == false)
				{
					poolData->SetConnect(true);
					m_useConnectionCount++;
					break;
				}
			}

			if (poolData == nullptr)
				return "";

			return EncodePoolID(i);
		}

		bool DisConnectPool(ComplexString poolID)
		{
			int poolIdx = DecodePoolID(poolID);

			if (poolIdx == -1)
				return false;

			if (poolIdx > (m_pool.size() - 1))
				return false;

			ComplexPoolData<T>* poolData = m_pool.at(poolIdx);
			poolData->SetConnect(false);
			m_useConnectionCount--;

			return true;
		}

		T* GetInstance(ComplexString poolID)
		{
			ComplexPoolData<T>* poolData = GetPoolData(poolID);

			if (poolData == nullptr)
				return nullptr;

			T* t = poolData->GetInstance();

			return t;
		}

		ComplexPoolData<T>* GetPoolData(ComplexString poolID)
		{
			int poolIdx = DecodePoolID(poolID);
			if (poolIdx == -1)
				return nullptr;

			if (poolIdx > (m_pool.size() - 1))
				return nullptr;

			ComplexPoolData<T>* poolData = m_pool.at(poolIdx);

			return poolData;
		}

		void ReservePoolSize(int modifySize)
		{
			if (modifySize > MAX_POOL_CAPACITY)
				throw ComplexCapacityOverflowException("pool insert capacity is maxsize.", "ComplexPool", "ReservePoolSize");
			else if (modifySize < 1)
				throw ComplexIndexOutOfBoundsException("pool insert capacity is at least 1.", "ComplexPool", "ReservePoolSize");

			int size_error = m_poolCapacity - modifySize;

			// 사이즈를 줄일 때
			if (size_error > 0)
			{
				for (int i = 0; i < size_error; i++)
				{
					m_pool.pop_back();
				}
			}
			// 사이즈를 늘릴 때
			else if (size_error < 0)
			{
				size_error = abs(size_error);
				for (int i = 0; i < size_error; i++)
				{
					ComplexPoolData<T>* newPoolData = new ComplexPoolData<T>;
					m_pool.push_back(newPoolData);
				}
			}

		}

		int GetPoolSize() const
		{
			return m_poolCapacity;
		}

		int GetUseConnectionCount() const
		{
			return m_useConnectionCount;
		}

		PoolDataFactory* GetPoolDataFactory()
		{
			return &m_pool;
		}

	private:

		// pid 는 PIDxxx 형태로 저장
		ComplexString EncodePoolID(int id)
		{
			if (id > MAX_POOL_CAPACITY)
				return "";
			ComplexString text = "PID";
			text.AppendFormat(MAX_POOL_CAPACITY_FORMAT, id);
			return text;
		}

		int DecodePoolID(ComplexString id)
		{
			if (id == "")
				return -1;

			if (id.GetLength() < MAX_POOL_CAPACITY_LENGTH)
				return -1;

			int findIdx = id.Find("PID");

			if (findIdx == -1)
				return -1;

			ComplexString id_num = id.GetText(3, id.GetLength());

			int returnID = -1;
			if (id_num != "")
				returnID = ComplexConvert::StringToInt(id_num);

			return returnID;
		}

	private:

		PoolDataFactory m_pool;
		int m_poolCapacity;
		int m_useConnectionCount;
	};
}