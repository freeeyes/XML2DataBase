#ifndef _LRU_SRC_H
#define _LRU_SRC_H

//用双向链表实现LRU的记录与淘汰
//add by freeeyes

#include <vector>
#include <assert.h>
#include "ThreadLock.h"
#include <ext/hash_map>
using namespace __gnu_cxx;
using namespace std;

#define MAX_DEFAULT_SIZE 1024

//回调函数指针声明

template <class K, class D>
struct Node
{
    K  key;
    D* data;
    Node* prev, *next;
};

template <class K, class D>
class LRUCache
{
public:
	LRUCache()
	{
		m_fnc_callback_eliminate = NULL;
        m_fnc_callback_reset_object = NULL;
	}

    LRUCache(size_t size, bool is_pthread_safe = false)
    {
        Init(size, is_pthread_safe);
    }

    ~LRUCache()
    {
        delete head;
        delete tail;
        delete[] entries;
    }
    
	void Init(size_t size, bool is_pthread_safe = false)
	{
		if(size <= 0)
		{
			size = MAX_DEFAULT_SIZE;
		}

        pthread_safe = is_pthread_safe;

        entries = new Node<K,D>[size];

		for(size_t i = 0; i < size; ++i)  
		{
			cached_entries.push_back(entries + i);  
		}

        head = new Node<K,D>;
        tail = new Node<K,D>;
        head->prev = NULL;
        head->next = tail;
        tail->prev = head;
        tail->next = NULL;
	}

    void Put(K key, D* data)
    {
        if(pthread_safe == true)
        {
            CThreadLock objThreadLock;

            Node<K,D>* node = cached_map[key];

            if(node)
            {
                detach(node);
                node->data = data;
                attach(node);
            }
            else
            {
                if(cached_entries.empty())
                {
                    node = tail->prev;
					if(NULL != m_fnc_callback_eliminate)
					{
						m_fnc_callback_eliminate(node->data);
					}
                    detach(node);
                    cached_map.erase(node->key);
                }
                else
                {
                    node = cached_entries.back();
                    cached_entries.pop_back();
                }

                node->key = key;
                node->data = data;
                cached_map[key] = node;
                attach(node);
            }
        }
        else
        {
            Node<K,D>* node = cached_map[key];

            if(node)
            {
                detach(node);
                node->data = data;
                attach(node);
            }
            else
            {
                if(cached_entries.empty())
                {
                    node = tail->prev;
					if(NULL != m_fnc_callback_eliminate)
					{
						m_fnc_callback_eliminate(node->data);
					}
                    detach(node);
                    cached_map.erase(node->key);
                }
                else
                {
                    node = cached_entries.back();
                    cached_entries.pop_back();
                }

                node->key = key;
                node->data = data;
                cached_map[key] = node;
                attach(node);
            }
        }
    }
    
	D* Get_Tail(K key)
	{
		Node<K,D>* node = NULL;
		node = tail->prev;
		D* pData = node->data;
		if(NULL != m_fnc_callback_eliminate)
		{
			m_fnc_callback_eliminate(pData);
            m_fnc_callback_reset_object(pData);
		}
		detach(node);
		cached_map.erase(node->key);

		//把这个指针作为新元素加入
		node->key = key;
		node->data = pData;
		cached_map[key] = node;
		attach(node);
		return pData;
	}

    D* Get(K key)
    {
        if(pthread_safe == true)
        {
            CThreadLock objThreadLock;

            Node<K,D>* node = cached_map[key];

            if(node)
            {
                detach(node);
                attach(node);
                return node->data;
            }
            else
            {
                return NULL;
            }
        }
        else
        {
            Node<K,D>* node = cached_map[key];

            if(node)
            {
                detach(node);
                attach(node);
                return node->data;
            }
            else
            {
                return NULL;
            }
        }
    }

    void Free_Node(K key)
    {
        if(pthread_safe == true)
        {
            CThreadLock objThreadLock;

            Node<K,D>* node = cached_map[key];

            if(node)
            {
                detach(node);
                cached_map.erase(node->key);
                m_fnc_callback_reset_object(node->data);
                return ;
            }
            else
            {
                return;
            }
        }
        else
        {
            Node<K,D>* node = cached_map[key];

            if(node)
            {
                detach(node);
                cached_map.erase(node->key);
                m_fnc_callback_reset_object(node->data);
                return ;
            }
            else
            {
                return;
            }
        }
    }

	typedef int (*callback_eliminate)(D* pData);

	void Set_CallBack_Func(callback_eliminate fnc_callback_eliminate)
	{
		m_fnc_callback_eliminate = fnc_callback_eliminate;
	}

    typedef void (*callback_reset_object)(D* pData);
    void Set_CallBack_Func(callback_reset_object fnc_callback_reset_object)
	{
		m_fnc_callback_reset_object = fnc_callback_reset_object;
	}
    
private:
    void detach(Node<K,D>* node)
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    void attach(Node<K,D>* node)
    {
        node->prev = head;
        node->next = head->next;
        head->next = node;
        node->next->prev = node;
    }

private:
    hash_map<K, Node<K,D>* > cached_map;
    vector<Node<K,D>* > cached_entries;
    Node<K,D>* head, *tail;
    Node<K,D>* entries;
    bool pthread_safe;

	callback_eliminate m_fnc_callback_eliminate;
    callback_reset_object m_fnc_callback_reset_object;
};

#endif
