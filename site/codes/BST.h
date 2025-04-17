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

	// queue 구조를 활용해서 계층관계로 구성된 트리의 노드들을 레벨순회를 한다.
	// stack 구조를 활용하면 깊이우선탐색 방식으로 노드를 접근할 수 있다.
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

	// 이번에 입력된 데이터가 첫번째라면
	if (nullptr == m_Root)
	{
		m_Root = pNewNode;
	}

	// BST 가 이미 데이터를 1개이상 보유하고 있다면
	else
	{
		// 새로 입력된 데이터의 First 값을 이용해서,
		// 루트노드부터 출발, 자신이 연결될 자리가 어딘지 찾아가야한다.
		tBSTNode<T1, T2>* pNode = m_Root;

		while (pNode)
		{
			// 노드에 들어있는 값보다 작으면, 왼쪽으로
			if (pNewNode->pair.first < pNode->pair.first)
			{
				// 왼쪽이 비어있으면, 그곳에 연결
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

			// 비교할 노드에 들어있는 fisrt 값보다 크면, 오른쪽으로
			else if (pNode->pair.first < pNewNode->pair.first)
			{
				// 오른쪽이 비어있으면, 그곳에 연결
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

			// 노드에 들어있는 first 값과 입력된 first 값이 같다
			else
			{
				assert(nullptr);
			}

		}
	}

	++m_Size;
}
