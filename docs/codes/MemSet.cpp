typedef unsigned char BYTE;

// Initializes the specified memory area to 0
// Uses void* so that the function can accept any variable's address
// Since void* is a pointer that cannot interpret the type of the destination,
// we additionally receive the size of the memory to be initialized
void MemSet(void* _pData, int _Size)
{
	BYTE* pData = (BYTE*)_pData;

	for (int i = 0; i < _Size; ++i)
	{		
		*(pData + i) = 0;
	}
}

// Moves data from one memory area to another
void MemMove(void* _Dest, int _Size, void* _Src)
{
	BYTE* pDest = (BYTE*)_Dest;
	BYTE* pSrc = (BYTE*)_Src;
	
	for (int i = 0; i < _Size; ++i)
	{
		//*(pDest + i) = *(pSrc + i);
		pDest[i] = pSrc[i];
	}
}
