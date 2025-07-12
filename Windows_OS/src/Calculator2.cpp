// Calculator2.cpp

#include <stdio.h>
#include <tchar.h>
#include <windows.h>

enum { DIV = 1, MUL, ADD, MIN, ELSE, EXIT };

DWORD ShowMenu();
void Divide(double, double);
void Multiply(double, double);
void Add(double, double);
void Min(double, double);

int _tmain(int argc, TCHAR* argv[]) {
	STARTUPINFO si = { 0, };
	PROCESS_INFORMATION pi;
	si.cb = sizeof(si);

	TCHAR command[] = _T("calc.exe");
	SetCurrentDirectory(_T("C:\\WINDOWS\\System32"));

	DWORD sel;
	double num1, num2;
	while (true) {
		sel = ShowMenu();
		if (sel == EXIT) return 0;

		if (sel != ELSE) {
			_fputts(_T("Input Num1 Num2 : "), stdout);
			_tscanf(_T("%lf, %lf"), &num1, &num2);
		}

		switch (sel) {
		case DIV:
			Divide(num1, num2);
			break;
		case MUL:
			Multiply(num1, num2);
			break;
		case ADD:
			Add(num1, num2);
			break;
		case MIN:
			Min(num1, num2);
			break;
		case ELSE:
			ZeroMemory(&pi, sizeof(pi));
			CreateProcess(
				NULL, command, NULL, NULL,
				TRUE, 0, NULL, NULL, &si, &pi
			);
			// CloseHandle(pi.hProcess); 
			// CloseHandle(pi.hThread); 
			break;
		}
	}

	return 0;
}

DWORD ShowMenu() {
	DWORD sel;
	_fputts(_T("-----MENU----- \n"), stdout);
	_fputts(_T("num1: Divide \n"), stdout);
	_fputts(_T("num2: Multiple \n"), stdout);
	_fputts(_T("num3: Add \n"), stdout);
	_fputts(_T("num4: Minus \n"), stdout);
	_fputts(_T("num5: Any other operations \n"), stdout);
	_fputts(_T("num6: Exit \n"), stdout);
	_fputts(_T("SELECTION >>"), stdout);
	_tscanf(_T("%d"), &sel);

	return sel;
}

void Divide(double a, double b) {
	if (b == 0) {
		_tprintf(_T("Error: Division by zero!\n\n"));
	}
	else {
		_tprintf(_T("%.2f / %.2f = %.2f \n\n"), a, b, a / b);
	}
}

void Multiply(double a, double b) {
	_tprintf(_T("%.2f * %.2f = %.2f \n\n"), a, b, a * b);
}

void Add(double a, double b) {
	_tprintf(_T("%.2f + %.2f = %.2f \n\n"), a, b, a + b);
}

void Min(double a, double b) {
	_tprintf(_T("%.2f - %.2f = %.2f \n\n"), a, b, a - b);
}