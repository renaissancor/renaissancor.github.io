#pragma once

#include <assert.h>

template<typename T>
struct tNode
{
	T		m_Data;	// 노드안에 저장된 데이터
	tNode*	m_Next;	// 다음 노드를 가리킴
	tNode*	m_Prev; // 이전노드를 가리킴

	
public:
	tNode()
		: m_Data()
		, m_Next(nullptr)
		, m_Prev(nullptr)
	{}

	tNode(const T& _Data, tNode* _Next = nullptr, tNode* _Prev = nullptr)
		: m_Data(_Data)
		, m_Next(_Next)
		, m_Prev(_Prev)
	{}
};

template<typename T>
class CList
{
private:
	tNode<T>*	m_Head;	// 시작데이터를 가지고있는 노드를 가리킴
	tNode<T>*	m_Tail; // 끝에 있는 데이터를 가지고있는 노드를 가리킴
	int			m_Size; // 보유하고 있는 데이터 개수

public:
	int size() { return m_Size; }
	void clear();

	void push_back(const T& _Data);
	void push_front(const T& _Data);

	class iterator;
	iterator begin() { return iterator(this, m_Head); }
	iterator end() { return iterator(this, nullptr); }

public:
	CList()
		: m_Head(nullptr)
		, m_Tail(nullptr)
		, m_Size(0)
	{}

	~CList()
	{
		clear();
	}

public:
	class iterator
	{
	private:
		CList<T>*	m_Owner;		// 데이터를 저장 및 관리하는 list 객체
		tNode<T>*	m_TargetNode;	// list 객체가 보유하고 있는 데이터를, 저장하고있는 노드

		// m_Owner == nullptr && m_TargetNode == nullptr ==> 아무것도 가리키지 않는 상태
		// m_Owner != nullptr && m_TargetNode == nullptr ==> List 보유 데이터의 마지막 다음을 가리키는 end iterator 상태

	public:
		bool operator ==(const iterator& _Other)
		{
			if (m_Owner == _Other.m_Owner && m_TargetNode == _Other.m_TargetNode)
				return true;
			return false;
		}

		bool operator != (const iterator& _Other)
		{
			return !((*this) == _Other);
		}

		iterator& operator++()
		{
			// iterator 가 아무것도 안가리키고 있거나, 
			// end iterator 상태면 다음을 가리킬 수 없다.
			assert(m_TargetNode);

			m_TargetNode = m_TargetNode->m_Next;

			return *this;
		}



		T& operator*()
		{
			assert(m_TargetNode);
			return m_TargetNode->m_Data;
		}

	public:
		iterator()
			: m_Owner(nullptr)
			, m_TargetNode(nullptr)
		{}
		iterator(CList<T>* _Owner, tNode<T>* _Node)
			: m_Owner(_Owner)
			, m_TargetNode(_Node)
		{}
		~iterator()
		{}
	};
};


template<typename T>
void CList<T>::push_back(const T& _Data)
{
	tNode<T>* pNewNode = new tNode<T>(_Data);

	// 데이터가 첫번째라면
	if (nullptr == m_Head)
	{
		// 데이터가 1개로 유일한 경우, 이 데이터는 시작이자 끝으로 볼 수 있다.
		m_Head = pNewNode;
		m_Tail = pNewNode;
	}

	// 데이터라 1개 이상 이미 있다.
	else
	{
		// List 의 Tail 이 가리키는 노드랑, 새로생긴 노드를 서로 연결해준다.
		m_Tail->m_Next = pNewNode;
		pNewNode->m_Prev = m_Tail;

		// 새로 생성된 노드를 새로운 Tail 로 지정한다.
		m_Tail = pNewNode;
	}

	++m_Size;
}

template<typename T>
void CList<T>::push_front(const T& _Data)
{
	// 입력된 데이터를 저장할 노드를 생성하고,
	// 생성된 노드는 기존 Head 노드를 자신의 다음으로 지정한다.
	tNode<T>* pNewNode = new tNode<T>(_Data, m_Head);
	
	// 새로 생성된 노드를 새로운 헤드로 지정한다.
	m_Head = pNewNode;

	// List 가 데이터를 1개 이상 원래 가지고 있었다면,
	if (1 <= m_Size)
	{
		// 기존 헤드노드가, 새로 생성된 노드를 자신의 이전으로 가리킨다.
		pNewNode->m_Next->m_Prev = pNewNode;
	}
	
	// 데이터 카운팅
	++m_Size;
}


template<typename T>
void CList<T>::clear()
{
	tNode<T>* pNode = m_Head;
	for (int i = 0; i < m_Size;++i)
	{
		tNode<T>* pNext = pNode->m_Next;
		delete pNode;
		pNode = pNext;
	}

	m_Head = nullptr;
	m_Tail = nullptr;
	m_Size = 0;
}