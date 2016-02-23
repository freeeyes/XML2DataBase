#ifndef _CATCH_POOL_H
#define _CATCH_POOL_H

#include "Lru.h"
#include <list>

template <class K, class D>
class CCachePool
{
public:
	CCachePool()
	{
		m_nMaxSize = 0;
	}

	~CCachePool()
	{
	}

	typedef bool (*callback_Check_Valid)(K& key, D* pData);
    typedef void (*callback_reset_object)(D* pData);
	void Init(D* pData, int nSize, callback_Check_Valid Fn_callback_Check_Valid, callback_reset_object Fn_callback_Reset_Object)
	{
		m_obj_LRUCache.Init(nSize);
		m_nMaxSize = nSize;
		for(int i = 0; i < nSize; i++)
		{
			K key;
			if(true == Fn_callback_Check_Valid(key, pData + i))
			{
				m_obj_LRUCache.Put(key, pData);
			}
			else
			{
				m_list_obj_catch.push_back(pData + i);
			}
		}

        m_obj_LRUCache.Set_CallBack_Func(Fn_callback_Reset_Object);
	}

    void Put(K key, D* data)
    {
        D* ptrData = Get(key);
        *ptrData = *data;
        m_obj_LRUCache.Put(key, ptrData);
    }

	//得到链表最后一个地址
	D* Get_Tail(K obj_Key)
	{
		return m_obj_LRUCache.Get_Tail(obj_Key);
	}

	D* Get(K obj_Key)
	{
		//首先查看，LRU中是否存在，如果有则直接返回。
		D* pLRUData = m_obj_LRUCache.Get(obj_Key);
		if(NULL != pLRUData)
		{
			return pLRUData;
		}

		//如果缓冲没有命中，则去寻找有没有空余
		if(m_list_obj_catch.size() > 0)
		{
			D* pData = NULL;
			pData = m_list_obj_catch.back();
			m_list_obj_catch.pop_back();
			m_obj_LRUCache.Put(obj_Key, pData);
			return pData;
		}
		else
		{
			//如果连空余也没有了，那么去进行LRU的淘汰
			return Get_Tail(obj_Key);
		}
	}

    void Free_Node(K key)
    {
        m_obj_LRUCache.Free_Node(key);
    }

	void Set_CallBack_Func(typename LRUCache<K, D>::callback_eliminate fnc_callback_eliminate)
	{
		m_obj_LRUCache.Set_CallBack_Func(fnc_callback_eliminate);
	}

private:
	LRUCache<K, D> m_obj_LRUCache;
	typedef list<D*> list_obj_catch;
	list_obj_catch m_list_obj_catch;
	int            m_nMaxSize;
};

#endif
