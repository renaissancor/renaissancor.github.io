#include <iostream>
#include <iomanip>

typedef unsigned char BYTE;

void MemSet(void* _pData, int _Size)
{
	BYTE* pData = (BYTE*)_pData;

	for (int i = 0; i < _Size; ++i)
	{
		*(pData + i) = 0;
	}
}

// Goal : Copy data from one memory area to another

void MemCopy(void* _Dest, int _Size, void* _Src) {
	BYTE* pDest = (BYTE*)_Dest;
	BYTE* pSrc = (BYTE*)_Src;
	for (int i = 0; i < _Size; ++i) {
		pDest[i] = pSrc[i];
	}
}

// Goal : Move data from one memory area to another 
// Note : The source and destination memory areas may overlap 

void MemMove(void* _Dest, int _Size, void* _Src)
{
	BYTE* pDest = (BYTE*)_Dest;
	BYTE* pSrc = (BYTE*)_Src;

	// Overlap happens when both src and dest are in the same memory area and they overlap, 
	// Meaning some block in the source is also in the destination 
	// When pDest < pSrc (destination before source): No overlap, so copying is safe. Data won't overwrite itself.
	// When pDest >= pSrc (destination after source): Overlap, so we need to copy from the end of the source to the beginning of the destination.
	// However, even when destination after source, but src + size is less than dest, it means NO OVERLAP, so copying is safe. 

	if (pDest < pSrc || pDest >= pSrc + _Size)
	{
		for (int i = 0; i < _Size; ++i)
		{
			//*(pDest + i) = *(pSrc + i);
			pDest[i] = pSrc[i];
		}
	}
	else
	{
		// Copy from the end of the source to the beginning of the destination
		for (int i = _Size - 1; i >= 0; --i)
		{
			//*(pDest + i) = *(pSrc + i);
			pDest[i] = pSrc[i];
		}
	}
}



int main() {
	// Test MemSet
	short arr[10];
	MemSet(arr, sizeof(arr));
	std::cout << "MemSet result:                 ";
	for (int i = 0; i < 10; i++) {
		std::cout << std::left << std::setw(3) << arr[i] << " ";
	}
	std::cout << "\n";

	// Test MemCopy
	int iArr[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	int iArrCopy[10];
	MemSet(iArrCopy, sizeof(iArrCopy));  // Initialize iArrCopy to zero
	MemCopy(iArrCopy, sizeof(iArr), iArr);
	std::cout << "MemCopy result:                ";
	for (int i = 0; i < 10; i++) {
		std::cout << std::left << std::setw(3) << iArrCopy[i] << " ";
	}
	std::cout << "\n";

	// Test MemMove (without overlap)
	int iArrOther[10] = {};
	MemSet(iArrOther, sizeof(iArrOther));  // Initialize iArrOther to zero

	MemMove(iArrOther, sizeof(iArr), iArr);
	std::cout << "MemMove result (no overlap):   ";
	for (int i = 0; i < 10; i++) {
		std::cout << std::left << std::setw(3) << iArrOther[i] << " ";
	}
	std::cout << "\n";

	// Test MemMove (with overlap)
	MemMove(iArr, sizeof(int) * 8, iArr + 2);
	std::cout << "MemMove result (with overlap): ";
	for (int i = 0; i < 10; i++) {
		std::cout << std::left << std::setw(3) << iArr[i] << " ";
	}
	std::cout << "\n";

	return 0;
}
