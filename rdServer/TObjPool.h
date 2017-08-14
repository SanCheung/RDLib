#pragma once

#include <list>
#include "Lock.h"

using namespace std;
using namespace winapi;

template<class T>
class TObjPool
{
public:

	TObjPool(void)
	{
		m_nMaxFree = 0;
	}

	~TObjPool(void)
	{
		Clear();
	}

public:
	T*	GetObj()
	{
		m_csObjs.Lock();
		T	*pObj = NULL;
		
		if( m_listFree.empty() )
			pObj = new T;
		else
		{
			list<T*>::iterator beginItem;

			beginItem = m_listFree.begin();
			pObj = *beginItem;
			m_listFree.pop_front();
		}
		
		ATLASSERT(pObj != NULL);
		m_listActive.push_back( pObj );
		
		m_csObjs.UnLock();
		return pObj;
	}

	void	ReleaseObj(T* pObj)
	{
		m_csObjs.Lock();
		if( pObj == NULL )
		{
			m_csObjs.UnLock();
			return;
		}
		
		m_listActive.remove( pObj );

		if( m_listFree.size() < m_nMaxFree )
			m_listFree.push_back( pObj );
		else
			delete pObj;
		
		m_csObjs.UnLock();
	}

	void Init(int nInitObjs, int nMaxFreeObjs)
	{
		for( int nIndex = 0; nIndex < nInitObjs; nIndex ++ )
		{
			T	*pObj = new T;
			m_listFree.push_back(pObj);
		}

		m_nMaxFree = nMaxFreeObjs;
	}

	void	Clear()
	{
		m_csObjs.Lock();
		
		T	*pObj = NULL;
		list<T*>::iterator item;

		for( item = m_listActive.begin(); item != m_listActive.end(); item ++ )
		{
			pObj = *item;
			delete pObj;
		}
		m_listActive.clear();
		
		for( item = m_listFree.begin(); item != m_listFree.end(); item ++ )
		{
			pObj = *item;
			delete pObj;
		}
		m_listFree.clear();
		
		m_csObjs.UnLock();
	}

protected:
	CCriticalSection	m_csObjs;
	list<T*>	m_listFree;
	list<T*>	m_listActive;
	size_t		m_nMaxFree;
};

