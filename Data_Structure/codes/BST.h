#pragma once

#include "CList.h"

enum NODE_TYPE
{
	PARENT,
	LCHILD,
	RCHILD,

	NONE,
};


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
	tBSTNode*		arrPtr[3];

public:
	bool HasLChild() { return nullptr != arrPtr[LCHILD]; }
	bool HasRChild() { return nullptr != arrPtr[RCHILD]; }	

	bool IsRoot() { return arrPtr[PARENT] == nullptr; }
	bool IsLeaf() { return (arrPtr[LCHILD] == nullptr) && (arrPtr[RCHILD] == nullptr); }
	bool IsFull() { return (arrPtr[LCHILD] != nullptr) && (arrPtr[RCHILD] != nullptr); }

	bool IsLChild() { return arrPtr[PARENT]->arrPtr[LCHILD] == this; }
	bool IsRchild() { return arrPtr[PARENT]->arrPtr[RCHILD] == this; }

	tBSTNode* GetNode(NODE_TYPE _Type) { return arrPtr[_Type]; }

public:
	tBSTNode()
		: pair()
		, arrPtr{}
	{}

	tBSTNode(const tPair<T1, T2>& _Pair, tBSTNode* _Parent = nullptr
			, tBSTNode* _LChild = nullptr, tBSTNode* _RChild = nullptr)
		: pair(_Pair)
		, arrPtr{ _Parent , _LChild, _RChild }
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

	tBSTNode<T1, T2>* GetInorderSuccessor(tBSTNode<T1, T2>* _Node);
	tBSTNode<T1, T2>* GetInorderPredecessor(tBSTNode<T1, T2>* _Node);

	class iterator;
	iterator begin()
	{
		if (!m_Root)
			return iterator(this, nullptr);

		// 루트에서 시작, 왼쪽으로 내려갈 수 없을때 까지 내려간다.
		tBSTNode<T1, T2>* pBeginNode = m_Root;		
		while (pBeginNode->arrPtr[LCHILD] != nullptr)
		{
			pBeginNode = pBeginNode->arrPtr[LCHILD];
		}
		return iterator(this, pBeginNode);
	}

	iterator end() { return iterator(this, nullptr); }

	iterator find(const T1& _Key);
	iterator erase(iterator& _iter);

public:
	BST()
		: m_Root(nullptr)
		, m_Size(0)
	{}
	~BST()
	{
		clear();
	}

	class iterator
	{
		friend class BST;
	private:
		BST<T1, T2>*		m_Owner;
		tBSTNode<T1, T2>*	m_Target; // EndIterator : nullptr == m_Target && m_Owner != nullptr


	public:
		bool operator==(const iterator& _Other)
		{
			return (m_Owner == _Other.m_Owner) && (m_Target == _Other.m_Target);
		}

		bool operator != (const iterator& _Other)
		{
			return !((*this) == _Other);
		}

		tPair<T1, T2>& operator*()
		{
			return m_Target->pair;
		}

		iterator& operator ++()
		{
			m_Target = m_Owner->GetInorderSuccessor(m_Target);	

			return *this;
		}


		iterator& operator --()
		{
			m_Target = m_Owner->GetInorderPredecessor(m_Target);

			return *this;
		}


	public:
		iterator()
			: m_Owner(nullptr)
			, m_Target(nullptr)
		{}
		iterator(BST<T1, T2>* _Owner, tBSTNode<T1, T2>* _Target)
			: m_Owner(_Owner)
			, m_Target(_Target)
		{}
		~iterator()
		{}
	};
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

		if (nullptr != pNode->arrPtr[LCHILD])
			queue.push_back(pNode->arrPtr[LCHILD]);
		if (nullptr != pNode->arrPtr[RCHILD])
			queue.push_back(pNode->arrPtr[RCHILD]);

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
			NODE_TYPE Type = NONE;

			// 노드에 들어있는 값보다 작으면, 왼쪽으로
			if (pNewNode->pair.first < pNode->pair.first)
				Type = LCHILD;
			// 비교할 노드에 들어있는 fisrt 값보다 크면, 오른쪽으로
			else if (pNode->pair.first < pNewNode->pair.first)
				Type = RCHILD;
			// 노드에 들어있는 first 값과 입력된 first 값이 같다
			else
				assert(nullptr);
						
			// 진행할 방향에 자리가 비었는지 확인
			if (nullptr == pNode->arrPtr[Type])
			{
				// 비어있으면, 해당 자리와 NewNode 를 연결
				pNode->arrPtr[Type] = pNewNode;
				pNewNode->arrPtr[PARENT] = pNode;
				break;
			}
			// 진행할 방향의 노드자리가 비어있지 않으면, 해당 자리 노드를 기준으로 다시 반복문 실행
			else
			{
				pNode = pNode->arrPtr[Type];
			}
		}
	}

	++m_Size;
}

template<typename T1, typename T2>
tBSTNode<T1, T2>* BST<T1, T2>::GetInorderSuccessor(tBSTNode<T1, T2>* _Node)
{
	// End 에서 다음 데이터를 요청한 경우
	assert(_Node);

	tBSTNode<T1, T2>* pSuccessor = nullptr;

	// 1. 오른쪽 자식 확인, 왼쪽자식이 없을때까지 내려간다.
	if (_Node->HasRChild())
	{
		pSuccessor = _Node->GetNode(RCHILD);
		while (pSuccessor->GetNode(LCHILD)) { pSuccessor = pSuccessor->GetNode(LCHILD); }
	}

	// 2. 부모의 왼쪽자식인지 확인, 부모가 후속자
	// 왼쪽자식이 아니면, 부모의 왼쪽자식이 될때까지 올라가서 그 부모노드가 후속자
	else
	{
		pSuccessor = nullptr;

		// 부모노드가 있다면
		while (!_Node->IsRoot())
		{
			// 왼쪽자식인지 확인
			if (_Node->IsLChild())
			{
				// 그때 부모노드가 후속자노드
				pSuccessor = _Node->GetNode(PARENT);
				break;
			}
			
			// 아니면 다시 위로 올라감
			_Node = _Node->GetNode(PARENT);
		}
	}

	return pSuccessor;
}

template<typename T1, typename T2>
tBSTNode<T1, T2>* BST<T1, T2>::GetInorderPredecessor(tBSTNode<T1, T2>* _Node)
{

	return nullptr;
}

template<typename T1, typename T2>
typename BST<T1, T2>::iterator BST<T1, T2>::find(const T1& _Key)
{
	// 입력된 Key 값을 이용해서, 루트노드부터 출발, 
	// Key 값과 동일한 First 를 보유한 노드를 찾는다.
	tBSTNode<T1, T2>* pNode = m_Root;

	while (pNode)
	{
		NODE_TYPE Type = NONE;

		// 노드에 들어있는 값보다 작으면, 왼쪽으로
		if (_Key < pNode->pair.first)
			Type = LCHILD;
		// 비교할 노드에 들어있는 fisrt 값보다 크면, 오른쪽으로
		else if (pNode->pair.first < _Key)
			Type = RCHILD;
		// 노드에 들어있는 first 값과 입력된 first 값이 같다
		else
		{
			return iterator(this, pNode);
		}

		// 진행할 방향에 자리가 비었는지 확인
		if (nullptr == pNode->arrPtr[Type])
		{			
			break;
		}
		// 진행할 방향의 노드자리가 비어있지 않으면, 해당 자리 노드를 기준으로 다시 반복문 실행
		else
		{
			pNode = pNode->arrPtr[Type];
		}
	}

	return end();
}

template<typename T1, typename T2>
typename BST<T1, T2>::iterator BST<T1, T2>::erase(iterator& _iter)
{
	// 예외조건
	assert(_iter.m_Owner == this && _iter.m_Target != nullptr);

	tBSTNode<T1, T2>* pSuccessor = nullptr;

	// 입력으로 들어온 iterator 가 가리키는 대상을 삭제한다.
	// 1. 삭제할 노드가 리프(단말)노드인 경우
	if (_iter.m_Target->IsLeaf())
	{
		// 삭제할 노드의 중위후속자 노드를 찾아둔다.
		pSuccessor = GetInorderSuccessor(_iter.m_Target);

		// 노드를 삭제한다.		
		// 1. 삭제할 노드가 루트다.
		if (_iter.m_Target == m_Root)
		{
			m_Root = nullptr;
		}
		// 2. 삭제할 노드가 루트가 아니다.
		else
		{
			// 부모노드가 삭제할 노드를 가리키기 않게 한다.
			if (_iter.m_Target->IsLChild())
				_iter.m_Target->GetNode(PARENT)->arrPtr[LCHILD] = nullptr;
			else
				_iter.m_Target->GetNode(PARENT)->arrPtr[RCHILD] = nullptr;			
		}		

		delete _iter.m_Target;
		--m_Size;	
	}

	// 2. 자식이 1개인 경우
	else if (!_iter.m_Target->IsFull())
	{
		// 삭제할 노드의 중위후속자 노드를 찾아둔다.
		pSuccessor = GetInorderSuccessor(_iter.m_Target);

		// 노드를 삭제하고, 하나뿐인 자식을 삭제된 자리로 옮긴다.
		// 1. 삭제할 노드가 루트다.
		if (_iter.m_Target == m_Root)
		{
			if (_iter.m_Target->HasLChild())
				m_Root = _iter.m_Target->GetNode(LCHILD);				
			else
				m_Root = _iter.m_Target->GetNode(RCHILD);

			m_Root->arrPtr[PARENT] = nullptr;
		}

		// 2. 삭제할 노드가 루트가 아니라면
		else
		{
			NODE_TYPE ChildType = NODE_TYPE::NONE;
			NODE_TYPE TargetType = NODE_TYPE::NONE;

			if (_iter.m_Target->HasLChild())
				ChildType = LCHILD;
			else
				ChildType = RCHILD;

			if (_iter.m_Target->IsLChild())
				TargetType = LCHILD;
			else
				TargetType = RCHILD;

			// 삭제할 노드의 자식 노드와, 삭제할 노드의 부모노드를 연결해준다
			_iter.m_Target->GetNode(PARENT)->arrPtr[TargetType] = _iter.m_Target->GetNode(ChildType);
			_iter.m_Target->GetNode(ChildType)->arrPtr[PARENT] = _iter.m_Target->GetNode(PARENT);
		}

		delete _iter.m_Target;
		--m_Size;
	}

	// 3. 자식이 2개
	else
	{
		// 삭제될 노드의 중위 후속자를 찾아서 그 자리를 대체시킨다.
		// 1. 삭제할 노드의 중위후속자 노드를 찾아둔다.
		pSuccessor = GetInorderSuccessor(_iter.m_Target);

		// 2. 후속자 노드의 데이터를 삭제하려고 했던 노드로 옮긴다.
		_iter.m_Target->pair = pSuccessor->pair;

		// 3. 실제 삭제는 후속자 노드를 삭제한다.
		iterator pNextIter(this, pSuccessor);
		erase(pNextIter);

		// 4. 원래 삭제하려고 했던 노드가, 다음 데이터를 가지게 되었으므로, 다음 노드가 된다.
		pSuccessor = _iter.m_Target;
	}
		
	// 입력으로 들어온 iterator 를 사용할 수 없는 상태로 만든다.
	_iter.m_Owner = nullptr;
	_iter.m_Target = nullptr;

	// 삭제한 다음 노드를 가리키는 iterator 를 만들어서 반환한다.
	return iterator(this, pSuccessor);
}
