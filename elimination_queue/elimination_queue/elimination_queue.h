#ifndef __Elimination_Queue_H__
#define __Elimination_Queue_H__

#include "list_poolex.h"
#include <unordered_map>

namespace EliminationQueue
{
	template<typename T>
	class Queue
	{
		friend class TListPoolEx<T>;

		class QueueType
		{
		public:
			T m_tData;
		};
	public:
		typedef void OnElimination(T& ref);

		Queue(OnElimination* pfun) : m_pfunOnElimination(pfun) {}

		bool Init(unsigned int dwCount)
		{
			return m_tListPool.AllocT(dwCount);
		}

		bool AddToQueue(const T& ref)
		{
			this->Remove(ref);
			QueueType* qtData = m_tListPool.FetchObj();
			if (NULL == qtData)
			{
				QueueType* qtTemp = m_tListPool.Eliminate();
				this->m_pfunOnElimination(qtTemp->m_tData);
				m_mapData.erase(qtTemp->m_tData);
				qtData = m_tListPool.FetchObj();
			}

			assert(qtData);
			if (!qtData)
			{
				return false;
			}
			qtData->m_tData = ref;
			m_mapData[ref] = qtData;
			return true;
		}

		bool Remove(const T& ref)
		{
			typename std::unordered_map<T, QueueType*>::iterator it = m_mapData.find(ref);
			if (m_mapData.end() == it)
			{
				return false;
			}
			m_tListPool.ReleaseObj(it->second);
			m_mapData.erase(it);
			return true;
		}

	private:
		TListPoolEx<QueueType> m_tListPool;
		OnElimination* m_pfunOnElimination;

		std::unordered_map<T, QueueType*>	m_mapData;
	};


};

#endif // !__Elimination_Queue_H__

