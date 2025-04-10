#pragma once

#include <assert.h>

// 클래스 템플릿
template<typename T>
class CArray
{
private:
	T*		m_Data;
	int		m_Capacity;
	int		m_CurSize;

public:
	void push_back(const T& _Data);
	void reserve(int _Capacity);
	void resize(int _Size);
	void clear() { m_CurSize = 0; }

	class iterator;
	iterator begin()
	{		
		if (m_CurSize == 0)
			return end();
		else
			return iterator(this, 0);
	}

	iterator end()
	{
		return iterator(this, -1);
	}


	void swap(CArray& _Other)
	{
		T* pData = m_Data;
		m_Data = _Other.m_Data;
		_Other.m_Data = pData;

		int Temp = m_Capacity;
		m_Capacity = _Other.m_Capacity;
		_Other.m_Capacity = Temp;

		Temp = m_CurSize;
		m_CurSize = _Other.m_CurSize;
		_Other.m_CurSize = Temp;
	}


	int size() { return m_CurSize; }
	int capacity() { return m_Capacity; }

	T& at(int _idx) 
	{ 		
		assert(_idx < m_CurSize);
		return m_Data[_idx]; 
	}	
	
	T& operator[](int _idx) 
	{ 
		assert(_idx < m_CurSize);
		return m_Data[_idx]; 
	}

	CArray& operator =(const CArray<T>& _Other);
	

public:
	CArray();
	~CArray();

	// CArray 의 private 접근 가능
	class iterator
	{
	private:
		CArray<T>*	m_Owner;
		int			m_Idx;

		// m_Owner == nullptr && m_Idx == -1  ==> 아무것도 안가리키기고 있는 iterator
		// m_Onwer != nullptr && m_Idx == -1  ==> 특정 컨테이너의 end 지점을 가리키고 있는 상태

	public:
		T& operator*()
		{
			return m_Owner->m_Data[m_Idx];
		}

		iterator& operator ++()
		{	
			// End Iterator 에 ++ 을 호출한 경우
			assert(m_Idx != -1);

			++m_Idx;

			// 컨테이너가 보유한 데이터 개수를 초과해서 가리키게 됨 ==> End Iterator
			if (m_Owner->m_CurSize <= m_Idx)
			{
				m_Idx = -1;
			}

			return *this;
		}

		iterator operator ++(int _Data)
		{
			iterator copyiter(*this);

			++(*this);

			return copyiter;
		}

		iterator& operator --()
		{
			return *this;
		}

		iterator operator--(int)
		{
			
		}




		bool operator == (const iterator& _Other)
		{
			if (m_Owner == _Other.m_Owner && m_Idx == _Other.m_Idx)
				return true;

			return false;
		}

		bool operator !=(const iterator& _Other)
		{
			return !((*this) == _Other);
		}



	public:
		iterator()
			: m_Owner(nullptr)
			, m_Idx(-1)
		{}

		iterator(CArray<T>* _Owner, int _Idx)
			: m_Owner(_Owner)
			, m_Idx(_Idx)
		{}

		~iterator()
		{
		}
	};



};


template<typename T>
CArray<T>::CArray()
	: m_Data(nullptr)
	, m_Capacity(0)
	, m_CurSize(0)
{	
}

template<typename T>
CArray<T>::~CArray()
{
	delete[] m_Data;
}

template<typename T>
void CArray<T>::push_back(const T& _Data)
{
	// 현재 힙공간에 데이터가 꽉 찬 경우
	if (m_Capacity <= m_CurSize)
	{
		reserve(m_Capacity * 2);
	}

	m_Data[m_CurSize++] = _Data;
}

template<typename T>
void CArray<T>::reserve(int _Capacity)
{
	if (0 == _Capacity)
		_Capacity = 1;

	// 더 넓은 공간으로 데이터를 이전해야한다.
	T* pNewData = new T[_Capacity];

	for (int i = 0; i < m_CurSize; ++i)
	{
		pNewData[i] = m_Data[i];
	}

	delete[] m_Data;

	m_Data = pNewData;

	m_Capacity = _Capacity;
}

template<typename T>
void CArray<T>::resize(int _Size)
{
	// resize 수치가 기존크기보다 더 크면
	if (m_CurSize < _Size)
	{
		reserve(_Size);

		// 추가된 공간을 기본값으로 채움
		for (int i = 0; i < _Size - m_CurSize; ++i)
		{
			m_Data[m_CurSize + i] = T();
		}
	}

	// 변경된 사이즈로 갱신
	m_CurSize = _Size;	
}



template<typename T>
CArray<T>& CArray<T>::operator=(const CArray<T>& _Other)
{
	if (_Other.m_CurSize == 0)
	{
		m_CurSize = 0;
		return *this;
	}

	// 기존 공간을 삭제한다.
	if (nullptr != m_Data)
		delete[] m_Data;

	// 원본 배열이 보유한 데이터 개수만큼 동적할당을 한다.
	m_Data = new int[_Other.m_CurSize];

	// 원본배열이 보유한 데이터를, 새로 할당한 공간에 복사받는다.
	for (int i = 0; i < _Other.m_CurSize; ++i)
	{
		m_Data[i] = _Other.m_Data[i];
	}

	// Capacity, CurSize 갱신
	m_Capacity = _Other.m_CurSize;
	m_CurSize = _Other.m_CurSize;

	return *this;
}