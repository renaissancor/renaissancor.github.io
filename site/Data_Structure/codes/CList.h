#pragma once

#include <assert.h>

template<typename T>
struct tNode
{
	T		m_Data;	// ���ȿ� ����� ������
	tNode* m_Next;	// ���� ��带 ����Ŵ
	tNode* m_Prev; // ������带 ����Ŵ


public:
	tNode()
		: m_Data()
		, m_Next(nullptr)
		, m_Prev(nullptr)
	{
	}

	tNode(const T& _Data, tNode* _Next = nullptr, tNode* _Prev = nullptr)
		: m_Data(_Data)
		, m_Next(_Next)
		, m_Prev(_Prev)
	{
	}
};

template<typename T>
class CList
{
private:
	tNode<T>* m_Head;	// ���۵����͸� �������ִ� ��带 ����Ŵ
	tNode<T>* m_Tail; // ���� �ִ� �����͸� �������ִ� ��带 ����Ŵ
	int			m_Size; // �����ϰ� �ִ� ������ ����

public:
	int size() { return m_Size; }
	void clear();

	void push_back(const T& _Data);
	void push_front(const T& _Data);

	T& front() { assert(m_Head); return m_Head->m_Data; }
	T& back() { assert(m_Tail); return m_Tail->m_Data; }

	// ���� ù��° �����͸� ����
	void pop_front();

	// ���� ������ �����͸� ����
	void pop_back();

	void reverse()
	{
		reverse(m_Head);
	}

private:
	// Use Recursion Function Structure 
	void reverse(tNode<T>* _Node)
	{
		if (nullptr == _Node)
		{
			return; // Meaning one node exist 
		}
		else {
			// Call from first node to last node 
			// Tail fixed for the first element? 

			tNode<T> *pNext = _Node->m_Next; // Next node

			if (nullptr == _Node->m_Next)
			{
				tNode<T>* pHead = m_Head;
				m_Head = m_Tail;
				m_Tail = pHead;
			}
			else {
				reverse(pNext); // Call from first node to last node
			}

			_Node->m_Next = _Node->m_Prev; 
			_Node->m_Prev = pNext; 
		}
	}


public:
	class iterator;
	iterator begin() { return iterator(this, m_Head); }
	iterator end() { return iterator(this, nullptr); }
	iterator erase(iterator& _iter);




public:
	CList()
		: m_Head(nullptr)
		, m_Tail(nullptr)
		, m_Size(0)
	{
	}

	~CList()
	{
		clear();
	}

public:
	class iterator
	{
	private:
		CList<T>* m_Owner;		// �����͸� ���� �� �����ϴ� list ��ü
		tNode<T>* m_TargetNode;	// list ��ü�� �����ϰ� �ִ� �����͸�, �����ϰ��ִ� ���

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

		iterator operator++(int)
		{
			iterator copyiter(*this);
			++(*this);
			return copyiter;
		}


		iterator& operator--()
		{
			// Begin Iterator �� -- �� ȣ���� ���
			assert(m_Owner->m_Head != m_TargetNode);

			// End iterator �� -- �� ȣ���� ���
			if (nullptr == m_TargetNode)
			{
				// �����̳ʿ� �����Ͱ� �ϳ��� ����(�������� �� ������ ��尡 �������� �ʴ´�.)
				assert(m_Owner->m_Tail);

				m_TargetNode = m_Owner->m_Tail;
			}

			else
			{
				m_TargetNode = m_TargetNode->m_Prev;
			}

			return *this;
		}

		iterator operator--(int)
		{
			iterator copyiter(*this);
			--(*this);
			return copyiter;
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
		{
		}
		iterator(CList<T>* _Owner, tNode<T>* _Node)
			: m_Owner(_Owner)
			, m_TargetNode(_Node)
		{
		}
		~iterator()
		{
		}

		// CList Ŭ���� �ʿ��� iterator �� private ���� ���� �����ϰ� ������ش�.
		friend class CList;
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
inline void CList<T>::pop_front()
{
	// �����Ͱ� �ϳ��� ���µ� pop_front�� ȣ���� ���
	assert(m_Head);

	// �����Ͱ� 2�� �̻��� ���,
	if (nullptr != m_Head->m_Next)
	{
		// �ι�° ���� �����ؼ�, ������带 ����Ű���� �ʰ� �Ѵ�.
		m_Head->m_Next->m_Prev = nullptr;
	}

	// �����Ͱ� 1���ΰ��
	else
	{
		m_Tail = nullptr;
	}

	tNode<T>* pHead = m_Head;
	m_Head = m_Head->m_Next;
	delete pHead;

	// ������ ���� ī����
	--m_Size;
}

template<typename T>
void CList<T>::pop_back()
{
	// �����Ͱ� �ϳ��� ���µ� pop_back�� ȣ���� ���
	assert(m_Size);

	if (1 == m_Size)
	{
		pop_front();
		return;
	}

	// ������ ��带 ����Ű�� m_Tail �� ���� �ϳ� ���� ��带 ����Ű�� �Ѵ�.	
	m_Tail = m_Tail->m_Prev;

	// ���� ������ ��带 �����Ѵ�.
	delete m_Tail->m_Next;

	// ���ο� ������ ��尡 ������ ����Ű�� �����͸� nullptr �� ���´�.
	m_Tail->m_Next = nullptr;

	--m_Size;
}

template<typename T>
typename CList<T>::iterator CList<T>::erase(iterator& _iter)
{
	// �Է����� ���� iterator �� ����Ű�� ����� �����ϰ�
	// ������ ������ ����Ű�� itetator �� ���� ��ȯ���ش�.

	// iterator �� ����Ű�� �����͸� �����ϰ��ִ� �����̳ʰ� erase ȣ���� ������ �ƴϴ�.
	assert(_iter.m_Owner == this);


	// ������ų ��� ���� ��带 ����Ű�� iterator �� �����д�.
	iterator NextIter = iterator(this, _iter.m_TargetNode->m_Next);

	// ù��° ��带 ����
	if (m_Head == _iter.m_TargetNode)
	{
		pop_front();
	}

	// ������ ��带 ����
	else if (m_Tail == _iter.m_TargetNode)
	{
		pop_back();
	}

	// �߰��� �ִ� ��带 ����
	else
	{
		_iter.m_TargetNode->m_Prev->m_Next = _iter.m_TargetNode->m_Next;
		_iter.m_TargetNode->m_Next->m_Prev = _iter.m_TargetNode->m_Prev;
		delete _iter.m_TargetNode;
		--m_Size;
	}

	return NextIter;
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