#include <windows.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <tlhelp32.h>
#define THREADS_NUMBER 3

// Сортировка методом выбора
void sortArray(int* myArray, int size)
{
	int min;

	for (int i = 0; i < size - 1; i++)
	{
		min = myArray[i];
		for (int y = i + 1; y < size; y++)
		{
			if (min > myArray[y])
			{
				min = myArray[y];
				myArray[y] = myArray[i];
				myArray[i] = min;
			}
		}
	}
}

//Бинарный поиск
void findInArray(int* myArray, int key, int size)
{
	int left, right, search = -1;
	left = 0;
	right = size - 1;

	while (left <= right)
	{
		int mid = (left + right) / 2;
		if (key == myArray[mid])
		{
			search = mid;
			break;
		}
		if (key < myArray[mid])
			right = mid - 1;
		else
			left = mid + 1;
	}
	if (search == -1)
		std::cout << "Element not found\n";
	else
		std::cout << "Element #" << myArray[search] << "\n";
}

DWORD WINAPI Thread(LPVOID *lpParam)
{
	LPSTR fileName = (LPSTR)lpParam;
	SYSTEMTIME startTime, endTime;
	int count;
	std::cout << "Thread #" << GetCurrentThreadId() << " launched\n";
	GetSystemTime(&startTime);
	std::ifstream inputFile(fileName);
	if (inputFile.is_open())
	{
		count = 0;
		int temp;
		while (!inputFile.eof())
		{
			inputFile >> temp;
			count++;
		}
		count--;
		int *myArray = new int[count];
		inputFile.close();
		inputFile.open(fileName);
		for (int i = 0; i < count; i++)
			inputFile >> myArray[i];
		inputFile.close();
		sortArray(myArray, count);
		//Ищем значение 1028
		findInArray(myArray, 1028, count);
	}
	Sleep(5);
	GetSystemTime(&endTime);
	std::cout << "Thread #" << GetCurrentThreadId() << " terminated\n";
	std::cout << "Thread operation time: " << (endTime.wMilliseconds - startTime.wMilliseconds) << "\n\n";
	ExitThread(0);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RU");
	HANDLE hThreads[THREADS_NUMBER];
	LPSTR Files[3] =
	{
		(LPSTR)"16384_50.1",
		(LPSTR)"Y1024Z.3",
		(LPSTR)"8192_100.1"
	};
	for (int i = 0; i < THREADS_NUMBER; i++)
	{

		hThreads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread, (void*)&Files[i], 0, NULL);
		if (NULL == hThreads[i])
			std::cout << "Ошибка создания потока.\n";

		Sleep(100);
	}

	WaitForMultipleObjects(THREADS_NUMBER, hThreads, TRUE, INFINITE);

	for (int i = 0; i < THREADS_NUMBER; i++)
		CloseHandle(hThreads[i]);


	HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	int count = 0;
	THREADENTRY32 te;
	if (h != INVALID_HANDLE_VALUE)
	{
		te.dwSize = sizeof(te);
		if (Thread32First(h, &te))
		{
			do
			{
				if (te.dwSize >= FIELD_OFFSET(THREADENTRY32, th32OwnerProcessID) + sizeof(te.th32OwnerProcessID))
					count++;
				te.dwSize = sizeof(te);
			} while (Thread32Next(h, &te));
		}
		CloseHandle(h);
	}
	std::cout << "Amount of descriptors: " << count << "\n";

	system("PAUSE");
	ExitProcess(0);
}