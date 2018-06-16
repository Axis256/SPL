#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>

int main()
{
	ShowWindow(GetForegroundWindow(), SW_HIDE);
	while (true)
	{
		if (GetAsyncKeyState(VK_ESCAPE))
		{
			MessageBox(NULL, L"Program terminated", L"Termination", MB_OK); return 0;
		}
		if (GetAsyncKeyState('E')) {
			int reg1, reg2, reg3, reg4;
			short reg5, reg6, reg7, reg8, reg9, reg10, reg11, reg12, reg13, reg14, reg15, reg16;
			__asm mov reg1, eax;
			__asm mov reg2, ebx;
			__asm mov reg3, ecx;
			__asm mov reg4, edx;

			__asm mov reg5, bp;
			__asm mov reg6, sp;
			__asm mov reg7, si;	
			__asm mov reg8, di;

			//__asm mov reg9, flags;
			//__asm mov reg10, ip;e
			__asm mov reg11, cs;
			__asm mov reg12, ds;

			__asm mov reg13, es;
			__asm mov reg14, fs;
			__asm mov reg15, gs;
			__asm mov reg16, ss;
			wchar_t buffer[256];
			wsprintfW(buffer, L"EAX: %X\nEBX: %X\nECX: %X\nEDX: %08X\nEBP: %X\nESP: %X\nESI: %X\nEDI: %X\nCS: %04X\nDS: %04X\nES: %04X\nFS: %04X\nGS: %04X\nSS: %04X\n", reg1, reg2, reg3, reg4, reg5, reg6, reg7, reg8, reg11, reg12, reg13, reg14, reg15, reg16);
			MessageBox(NULL, buffer, L"Registers' contents", MB_OK);
		}
	}
}