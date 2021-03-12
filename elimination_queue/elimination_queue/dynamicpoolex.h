#ifndef __DYNAMICPOOLEX_H_2009_1203__
#define __DYNAMICPOOLEX_H_2009_1203__

#include <list>
#include <deque>

template <class T>
class TDynamicPoolEx
{
public:
    explicit TDynamicPoolEx(void)
        : m_pFreeNode(0)
        , m_pUsedNode(0)
	{
    }

    virtual ~TDynamicPoolEx(void)
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
        if (0 != poNode)
        {
            //将节点摘掉
            m_pFreeNode = m_pFreeNode->m_pNext;
            m_pFreeNode->m_pPre = NULL;
            return NULL;
        }
        //从空闲列表中移除
        m_pFreeNode = m_pFreeNode->m_pNext;
        m_pFreeNode->m_pPre = NULL;
        //放入已使用列表中
        poNode->m_pNext = m_pUsedNode;
        m_pUsedNode->m_pPre = poNode;
        poNode->m_pPre = NULL;
        m_pUsedNode = poNode;

        ++poNode->m_nRef;
        return poNode;
    }

    void ReleaseObj(T* pObj)
    {
        if (NULL == pObj)
        {
            return;
        }

        Node* poShellT = dynamic_cast<Node*>(pObj);
        if (NULL == poShellT)
        {
            return;
        }
        //TODO



		// 如果此时引用计数不为1，也即对其使用出错，
		// 则不应该对其进行释放，调用他的析构函数和放入池
        if (0 != poShellT->m_nRef - 1)
        {
            return;
        }

		// 每次释放就对引用计数减1
        poShellT->m_nRef--;

		// 显示调用其析构函数
        m_ListFree.push_back(poShellT);
    }

    size_t GetFreeCount(void)
    {
        size_t n = 0;
        n = m_ListFree.size();
        return n;
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

        if(poNode == NULL)
        {
            return false;
        }

        m_pFreeNode = poNode;
        Node* pTemp = m_pFreeNode;

        for(unsigned int i = 1; i < dwCount; ++i)
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
        Node() : m_pNext(0), m_pPre(0), m_nRef(0){}
        ~Node(){}
        int m_nRef;
        Node* m_pNext;
        Node* m_pPre;
    };

	std::deque<Node*> m_ListFree;
	std::deque<Node*> m_ListAll;

    Node* m_pFreeNode;
    Node* m_pUsedNode;
};

#endif  // __DYNAMICPOOLEX_H_2009_1203__
