#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

typedef struct
{
	unsigned char lastName[64];
	int term;
	unsigned char discipline[128];
	int mark;

} Info;

void CreateBinaryFile(int n)
{
	Info* information;
	int i;
	FILE *file;

	file = fopen("File1.dat", "wb");
	information = (Info*)malloc(sizeof(Info) * n);
	srand(time(0));
	for (i = 0; i < n; i++)
	{
		strcpy(information[0].discipline, "���������� ���������� ");
		strcpy(information[0].lastName, "���������");
		information[0].mark = 3;
		information[0].term = 1;

		strcpy(information[1].discipline, "������� ");
		strcpy(information[1].lastName, "���������");
		information[1].mark = 4;
		information[1].term = 1;

		strcpy(information[2].discipline, "����������� ");
		strcpy(information[2].lastName, "���������");
		information[2].mark = 3;
		information[2].term = 1;

		strcpy(information[3].discipline, "����� � ��������� ���������������� ");
		strcpy(information[3].lastName, "���������");
		information[3].mark = 3;
		information[3].term = 1;

		strcpy(information[4].discipline, "���������� ");
		strcpy(information[4].lastName, "���������");
		information[4].mark = 3;
		information[4].term = 1;

		strcpy(information[5].discipline, "���������������� ");
		strcpy(information[5].lastName, "���������");
		information[5].mark = 4;
		information[5].term = 2;

		strcpy(information[6].discipline, "������ ");
		strcpy(information[6].lastName, "���������");
		information[6].mark = 3;
		information[6].term = 2;

		strcpy(information[7].discipline, "���������� ���������� ");
		strcpy(information[7].lastName, "���������");
		information[7].mark = 3;
		information[7].term = 2;

		strcpy(information[8].discipline, "���������� ");
		strcpy(information[8].lastName, "���������");
		information[8].mark = 4;
		information[8].term = 2;

		strcpy(information[9].discipline, "���� ������ ");
		strcpy(information[9].lastName, "���������");
		information[9].mark = 4;
		information[9].term = 2;

		strcpy(information[10].discipline, "������ ");
		strcpy(information[10].lastName, "���������");
		information[10].mark = 3;
		information[10].term = 3;

		strcpy(information[11].discipline, "���� ���������� ");
		strcpy(information[11].lastName, "���������");
		information[11].mark = 3;
		information[11].term = 3;

		strcpy(information[12].discipline, "���������� ");
		strcpy(information[12].lastName, "���������");
		information[12].mark = 4;
		information[12].term = 3;
	}

	fwrite(information, sizeof(Info), n, file);
	fclose(file);
}

void DeleteFiles() {
	remove("File1.dat");
	remove("Copy1File1.dat");
	remove("Copy2File1.dat");
	remove("Copy3File1.dat");
}

void FileCopyC()
{
	Info rec;
	FILE* in;
	FILE* out;
	int sumMark = 0;
	in = fopen("File1.dat", "rb");
	out = fopen("Copy1File1.dat", "wb");
	while (fread(&rec, sizeof(Info), 1, in) > 0)
		fwrite(&rec, sizeof(Info), 1, out);
	fclose(in);
	fclose(out);
}

void FileCopyWin()
{
	const TCHAR File1[] = L"File1.dat";
	const TCHAR Copy2File1[] = L"Copy2File1.dat";
	HANDLE hIn, hOut;
	DWORD nIn, nOut;
	Info buf;
	int sumMark = 0;
	hIn = CreateFile(File1, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	hOut = CreateFile(Copy2File1, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	while (ReadFile(hIn, &buf, sizeof(Info), &nIn, NULL) && nIn > 0)
		WriteFile(hOut, &buf, nIn, &nOut, NULL);
	CloseHandle(hOut);
	CloseHandle(hIn);
}

void FileCopyCopyFile()
{
	const TCHAR File1[] = L"File1.dat";
	const TCHAR Copy3File1[] = L"Copy3File1.dat";
	CopyFile(File1, Copy3File1, FALSE);
}

void printTop()
{
	int i;
	putchar(218);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(194);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(194);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(194);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(191);
	printf("\n%c%-10s%c%-10s%c%-10s%c%-10s%c\n", 179, "Type", 179, "Time (ms)", 179, "Time (ms)", 179, "Time (ms)", 179);
}

void printElems(char* first, int second, int third, int forth)
{
	printf("%c%-10s%c%-10i%c%-10d%c%-10d%c\n", 179, first, 179, second, 179, third, 179, forth, 179);
}

void printBorder() {
	int i;
	putchar(195);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(197);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(197);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(197);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(180);
	printf("\n");
}

void printBottom() {
	int i;
	putchar(192);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(193);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(193);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(193);
	for (i = 0; i < 10; i++)
		putchar(196);
	putchar(217);
	printf("\n");
}

int main() {

	clock_t start, end;
	long CFirst, CSecond, CThird;
	long WFirst, WSecond, WThird;
	long PFirst, PSecond, PThird;
	int size1 = 1000, size2 = size1 * 5, size3 = size2 * 5;

	{
		DeleteFiles();
		CreateBinaryFile(size1);

		start = clock(); FileCopyC(); end = clock();
		CFirst = (double)(end - start);

		start = clock(); FileCopyWin(); end = clock();
		WFirst = (double)(end - start);

		start = clock(); FileCopyCopyFile(); end = clock();
		PFirst = (double)(end - start);

		DeleteFiles();
		CreateBinaryFile(size2);

		start = clock(); FileCopyC(); end = clock();
		CSecond = (double)(end - start);

		start = clock(); FileCopyWin(); end = clock();
		WSecond = (double)(end - start);

		start = clock(); FileCopyCopyFile(); end = clock();
		PSecond = (double)(end - start);

		DeleteFiles();
		CreateBinaryFile(size3);

		start = clock(); FileCopyC(); end = clock();
		CThird = (double)(end - start);

		start = clock(); FileCopyWin(); end = clock();
		WThird = (double)(end - start);

		start = clock(); FileCopyCopyFile(); end = clock();
		PThird = (double)(end - start);
	}

	printTop();
	printBorder();
	printElems("C", CFirst, CSecond, CThird);
	printBorder();
	printElems("Windows", WFirst, WSecond, WThird);
	printBorder();
	printElems("CopyFile", PFirst, PSecond, PThird);
	printBorder();
	printElems("Size", size1, size2, size3);
	printBottom();
	system("pause");
	return 0;
}
