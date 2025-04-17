#pragma once

#include "CList.h"


template<typename T1, typename T2>
struct tPair
{
	T1	first;
	T2	second;
};

template<typename T1, typename T2>
struct tBSTNode
{
	tPair<T1, T2>	pair;
	tBSTNode*		pParent;
	tBSTNode*		pLChild;
	tBSTNode*		pRChild;

public:
	tBSTNode()
		: pair()
		, pParent(nullptr)
		, pLChild(nullptr)
		, pRChild(nullptr)
	{}

	tBSTNode(const tPair<T1, T2>& _Pair, tBSTNode* _Parent = nullptr
			, tBSTNode* _LChild = nullptr, tBSTNode* _RChild = nullptr)
		: pair(_Pair)
		, pParent(_Parent)
		, pLChild(_LChild)
		, pRChild(_RChild)
	{}
};


template<typename T1, typename T2>
tPair<T1, T2> MakePair(const T1& _First, const T2& _Second)
{
	tPair<T1, T2> pair;

	pair.first = _First;
	pair.second = _Second;

	return pair;
}



template<typename T1, typename T2>
class BST
{
private:
	tBSTNode<T1, T2>*	m_Root;
	int					m_Size;

public:
	int size() { return m_Size; }
	bool empty() { return m_Size == 0; }
	void clear();

	void insert(const tPair<T1, T2>& _pair);


public:
	BST()
		: m_Root(nullptr)
		, m_Size(0)
	{}

	~BST()
	{
		clear();
	}
};

template<typename T1, typename T2>
void BST<T1, T2>::clear()
{
	CList<tBSTNode<T1, T2>*> queue;

	if (nullptr != m_Root)
	{
		queue.push_back(m_Root);
	}

	// queue ������ Ȱ���ؼ� ��������� ������ Ʈ���� ������ ������ȸ�� �Ѵ�.
	// stack ������ Ȱ���ϸ� ���̿켱Ž�� ������� ��带 ������ �� �ִ�.
	while (!queue.empty())
	{
		tBSTNode<T1, T2>* pNode = queue.front();
		queue.pop_front();

		if (nullptr != pNode->pLChild)
			queue.push_back(pNode->pLChild);
		if (nullptr != pNode->pRChild)
			queue.push_back(pNode->pRChild);

		delete pNode;
	}

	m_Root = nullptr;
	m_Size = 0;
}

template<typename T1, typename T2>
void BST<T1, T2>::insert(const tPair<T1, T2>& _pair)
{
	tBSTNode<T1, T2>* pNewNode = new tBSTNode<T1, T2>(_pair);

	// �̹��� �Էµ� �����Ͱ� ù��°���
	if (nullptr == m_Root)
	{
		m_Root = pNewNode;
	}

	// BST �� �̹� �����͸� 1���̻� �����ϰ� �ִٸ�
	else
	{
		// ���� �Էµ� �������� First ���� �̿��ؼ�,
		// ��Ʈ������ ���, �ڽ��� ����� �ڸ��� ����� ã�ư����Ѵ�.
		tBSTNode<T1, T2>* pNode = m_Root;

		while (pNode)
		{
			// ��忡 ����ִ� ������ ������, ��������
			if (pNewNode->pair.first < pNode->pair.first)
			{
				// ������ ���������, �װ��� ����
				if (nullptr == pNode->pLChild)
				{
					pNode->pLChild = pNewNode;
					pNewNode->pParent = pNode;
					break;
				}
				else
				{
					pNode = pNode->pLChild;
				}
			}

			// ���� ��忡 ����ִ� fisrt ������ ũ��, ����������
			else if (pNode->pair.first < pNewNode->pair.first)
			{
				// �������� ���������, �װ��� ����
				if (nullptr == pNode->pRChild)
				{
					pNode->pRChild = pNewNode;
					pNewNode->pParent = pNode;
					break;
				}
				else
				{
					pNode = pNode->pRChild;
				}
			}

			// ��忡 ����ִ� first ���� �Էµ� first ���� ����
			else
			{
				assert(nullptr);
			}

		}
	}

	++m_Size;
}
