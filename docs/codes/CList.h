#pragma once

#include <assert.h>

template<typename T>
struct tNode
{
	T		m_Data;	// ���ȿ� ����� ������
	tNode*	m_Next;	// ���� ��带 ����Ŵ
	tNode*	m_Prev; // ������带 ����Ŵ

	
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
	tNode<T>*	m_Head;	// ���۵����͸� �������ִ� ��带 ����Ŵ
	tNode<T>*	m_Tail; // ���� �ִ� �����͸� �������ִ� ��带 ����Ŵ
	int			m_Size; // �����ϰ� �ִ� ������ ����

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
		CList<T>*	m_Owner;		// �����͸� ���� �� �����ϴ� list ��ü
		tNode<T>*	m_TargetNode;	// list ��ü�� �����ϰ� �ִ� �����͸�, �����ϰ��ִ� ���

		// m_Owner == nullptr && m_TargetNode == nullptr ==> �ƹ��͵� ����Ű�� �ʴ� ����
		// m_Owner != nullptr && m_TargetNode == nullptr ==> List ���� �������� ������ ������ ����Ű�� end iterator ����

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
			// iterator �� �ƹ��͵� �Ȱ���Ű�� �ְų�, 
			// end iterator ���¸� ������ ����ų �� ����.
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

	// �����Ͱ� ù��°���
	if (nullptr == m_Head)
	{
		// �����Ͱ� 1���� ������ ���, �� �����ʹ� �������� ������ �� �� �ִ�.
		m_Head = pNewNode;
		m_Tail = pNewNode;
	}

	// �����Ͷ� 1�� �̻� �̹� �ִ�.
	else
	{
		// List �� Tail �� ����Ű�� ����, ���λ��� ��带 ���� �������ش�.
		m_Tail->m_Next = pNewNode;
		pNewNode->m_Prev = m_Tail;

		// ���� ������ ��带 ���ο� Tail �� �����Ѵ�.
		m_Tail = pNewNode;
	}

	++m_Size;
}

template<typename T>
void CList<T>::push_front(const T& _Data)
{
	// �Էµ� �����͸� ������ ��带 �����ϰ�,
	// ������ ���� ���� Head ��带 �ڽ��� �������� �����Ѵ�.
	tNode<T>* pNewNode = new tNode<T>(_Data, m_Head);
	
	// ���� ������ ��带 ���ο� ���� �����Ѵ�.
	m_Head = pNewNode;

	// List �� �����͸� 1�� �̻� ���� ������ �־��ٸ�,
	if (1 <= m_Size)
	{
		// ���� ����尡, ���� ������ ��带 �ڽ��� �������� ����Ų��.
		pNewNode->m_Next->m_Prev = pNewNode;
	}
	
	// ������ ī����
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