#ifndef __DYNAMICPOOLEX_H_2009_1203__
#define __DYNAMICPOOLEX_H_2009_1203__

#include <stdio.h>
#include <assert.h>

namespace EliminationQueue
{
	template <class T>
	class TListPoolEx
	{
	public:
		explicit TListPoolEx(void)
			: m_pFreeNode(0)
			, m_pLastUsedNode(0)
			, m_pFirstUsedNode(0)
		{
		}

		virtual ~TListPoolEx(void)
		{
		}

		// Max count表示Pool的最大值，如果传入0，则表示没有限制
		bool Init()
		{
			return AllocT(0);
		}

		T* FetchObj(void)
		{
			if (!m_pFreeNode)
			{
				return NULL;
			}

			Node* poNode = m_pFreeNode;
			assert(0 == poNode->m_nRef);
			if (0 != poNode->m_nRef)
			{
				//将节点摘掉
				m_pFreeNode = m_pFreeNode->m_pNext;
				if (m_pFreeNode)
				{
					m_pFreeNode->m_pPre = NULL;
				}
				return NULL;
			}
			//从空闲列表中移除
			m_pFreeNode = m_pFreeNode->m_pNext;
			if (m_pFreeNode)
			{
				m_pFreeNode->m_pPre = NULL;
			}
			//放入已使用列表中
			poNode->m_pPre = m_pLastUsedNode;
			if (m_pLastUsedNode)
			{
				m_pLastUsedNode->m_pNext= poNode;
			}
			poNode->m_pNext = NULL;
			m_pLastUsedNode = poNode;
			if (NULL == m_pFirstUsedNode)
			{
				m_pFirstUsedNode = poNode;
			}

			++poNode->m_nRef;
			return poNode;
		}

		void ReleaseObj(T* pObj)
		{
			if (NULL == pObj)
			{
				return;
			}

			Node* poNode = static_cast<Node*>(pObj);
			if (NULL == poNode)
			{
				return;
			}

			//特殊处理
			if (m_pFirstUsedNode == poNode)
			{
				m_pFirstUsedNode = m_pFirstUsedNode->m_pNext;
			}
			if (m_pLastUsedNode == poNode)
			{
				m_pLastUsedNode = poNode->m_pPre;
			}

			//摘除节点
			if (poNode->m_pPre)
			{
				poNode->m_pPre->m_pNext = poNode->m_pNext;
			}
			if (poNode->m_pNext)
			{
				poNode->m_pNext->m_pPre = poNode->m_pPre;
			}

			assert(1 == poNode->m_nRef);
			if (1 == poNode->m_nRef)
			{
				--poNode->m_nRef;

				poNode->m_pNext = m_pFreeNode;
				if (m_pFreeNode)
				{
					m_pFreeNode->m_pPre = poNode;
				}
				poNode->m_pPre = NULL;
				m_pFreeNode = poNode;
			}
			else
			{
			}
		}

		T* Eliminate()
		{
			Node* temp = this->m_pFirstUsedNode;
			this->ReleaseObj(this->m_pFirstUsedNode);
			return temp;
		}

		bool AllocT(unsigned int dwCount)
		{
			if (0 == dwCount)
			{
				return false;
			}
			if (m_pFreeNode)
			{
				return true;
			}

			Node* poNode = new Node[dwCount];

			if (poNode == NULL)
			{
				return false;
			}

			m_pFreeNode = poNode;
			Node* pTemp = m_pFreeNode;

			for (unsigned int i = 1; i < dwCount; ++i)
			{
				pTemp->m_pNext = poNode + i;
				poNode[i].m_pPre = pTemp;
				pTemp = poNode + i;
			}

			return true;
		}

	protected:
	private:
		// 增加了引用计数的壳
		class Node : public T
		{
		public:
			Node() : m_pNext(0), m_pPre(0), m_nRef(0) {}
			~Node() {}
			int m_nRef;
			Node* m_pNext;
			Node* m_pPre;
		};

		Node* m_pFreeNode;
		Node* m_pLastUsedNode;
		Node* m_pFirstUsedNode;
	};
};


#endif  // __DYNAMICPOOLEX_H_2009_1203__
