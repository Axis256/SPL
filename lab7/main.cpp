#include "stdafx.h"
#include "windows.h"
#include <iostream>
#include <fstream>
#include "process.h"
#include "time.h"
#include <string>
#include <iomanip>

using namespace std;

void criticalSections();
void mutex();
void semaphore();
void events();

DWORD WINAPI searchInArrayCS(void *);
DWORD WINAPI simpleArraySortCS(void *);
DWORD WINAPI naturalArraySortCS(void *);

DWORD WINAPI searchInArrayM(void *);
DWORD WINAPI simpleArraySortM(void *);
DWORD WINAPI naturalArraySortM(void *);

DWORD WINAPI searchInArrayS(void *);
DWORD WINAPI simpleArraySortS(void *);
DWORD WINAPI naturalArraySortS(void *);

DWORD WINAPI searchInArrayE(void *);
DWORD WINAPI simpleArraySortE(void *);
DWORD WINAPI naturalArraySortE(void *);

void readFromFile(const char *str);
void writeToFile(const char *str);
void simpleMergeSort(int a[], long lb, long ub);
void merge(int a[], long lb, long split, long ub);
void putHorizontalLine(int size, int type);
void print();

HANDLE hEvent, hMutex, hSemaphore; // Объявление дескрипторов события, мютекса и семафора
CRITICAL_SECTION critSect; //Объявляем указатель на критическую секцию

int symbol = 9;
const int THREADS_NUMBER = 3;
const int ARRAY_NUMBER = 16380;
int useArray[16380];
long cMax = 1;
long time1[4];

int main()
{

	InitializeCriticalSection(&critSect); // Инициализация критической секции, теперь она может быть использована потоками
	long startTime, finishTime;
	startTime = clock();
	criticalSections();
	finishTime = clock();
	time1[0] = finishTime - startTime;
	
	Sleep(2000);
	cout << endl;

	hMutex = CreateMutex(NULL, FALSE, L"Mutex1");
	startTime = clock();
	mutex(); 
	finishTime = clock();
	time1[1] = finishTime - startTime;

	Sleep(2000);
	cout << endl;

	hSemaphore = CreateSemaphore(NULL, cMax, cMax, NULL); //Инициализация семафора
				//Предоставляем возможность только 1 потоку обращаться к семафору
	
	startTime = clock();
	semaphore();
	finishTime = clock();
	time1[2] = finishTime - startTime;
	

	Sleep(3000);
	cout << endl;

	hEvent = CreateEvent(NULL, TRUE, FALSE, L"Event");

	startTime = clock();
	events();
	finishTime = clock();
	time1[3] = finishTime - startTime;

	Sleep(2000);
	cout << endl;

	print();
	system("pause");
}

void readFromFile(const char *str)
{
	ifstream in(str);

	if (in.is_open())
	{
		int temp;

		int i = 0;
		while (!in.eof())
		{
			in >> temp;
			useArray[i] = temp;
			i++;
		}

		in.close();
	}
	else cout << "Файл не найден.";
}

void writeToFile(const char *str)
{
	fstream inOut;

	inOut.open(str, ios::out);
	for (int j = 0; j < 16380; j++)
	{
		if ((j != 0) && (j % 10 == 0)) inOut << "\n";
		inOut << useArray[j] << "\t";
	}
	inOut.close();
}

void simpleMergeSort(int a[], long lb, long ub) {
	long split;

	if (lb < ub) {

		split = (lb + ub) / 2;

		simpleMergeSort(a, lb, split);
		simpleMergeSort(a, split + 1, ub);
		merge(a, lb, split, ub);
	}
}

void merge(int a[], long lb, long split, long ub) {
	long pos1 = lb;
	long pos2 = split + 1;
	long pos3 = 0;

	int *temp = new int[ub - lb + 1];

	while (pos1 <= split && pos2 <= ub) {
		if (a[pos1] < a[pos2])
			temp[pos3++] = a[pos1++];
		else
			temp[pos3++] = a[pos2++];
	}

	while (pos2 <= ub)
		temp[pos3++] = a[pos2++];
	while (pos1 <= split)
		temp[pos3++] = a[pos1++];

	for (pos3 = 0; pos3 < ub - lb + 1; pos3++)
		a[lb + pos3] = temp[pos3];

}

void print()
{
	cout << "\n" << endl;
	putHorizontalLine(28, 1);
	cout << char(179) << " Method            " << char(179) << " Time " << char(179) << endl;
	putHorizontalLine(28, 0);
	cout << char(179) << " Critical sections " << char(179) << setw(5) << time1[0] << " " << char(179) << endl;
	putHorizontalLine(28, 0);
	cout << char(179) << " Mutex             " << char(179) << setw(5) << time1[1] << " " << char(179) << endl;
	putHorizontalLine(28, 0);
	cout << char(179) << " Semaphore         " << char(179) << setw(5) << time1[2] << " " << char(179) << endl;
	putHorizontalLine(28, 0);
	cout << char(179) << " Events            " << char(179) << setw(5) << time1[3] << " " << char(179) << endl;
	putHorizontalLine(28, 2);
}




//__________________________________________________________________
// КРИТИЧЕСКИЕ СЕКЦИИ
void criticalSections()
{
	readFromFile("16384_50.1");

	cout << "CRITICAL SECTIONS:\n";

	HANDLE hTHREADS[THREADS_NUMBER];
	CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//СОЗДАНИЕ ПОТОКОВ
	hTHREADS[0] = CreateThread(NULL, 0, simpleArraySortCS, NULL, 0, NULL); // сортировка простым слиянием
	hTHREADS[1] = CreateThread(NULL, 0, naturalArraySortCS, NULL, 0, NULL); // сортировка естественным двухпутевым слиянием
	hTHREADS[2] = CreateThread(NULL, 0, searchInArrayCS, NULL, 0, NULL); // поток поиска сортировки

	if ((hTHREADS[0] == NULL) || (hTHREADS[1] == NULL) || (hTHREADS[2] == NULL)) { return; }

	WaitForMultipleObjects(NULL,hTHREADS,TRUE, INFINITE);

	writeToFile("1.1");
	//ЗАКРЫВАЕМ ПОТОКИ
	for (int i = 0; i < THREADS_NUMBER; i++) {
		CloseHandle(hTHREADS[i]);
	}
}

//ПОИСК ПО ФАЙЛУ
DWORD WINAPI searchInArrayCS(void *)
{
	DWORD ID = GetCurrentThreadId();

	EnterCriticalSection(&critSect); //НАЧАЛО КРИТИЧЕСКОЙ СЕКЦИИ
	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	ifstream in("16384_50.1"); //происходит обращение к общему внешнему ресурсу - файлу

	if (in.is_open())
	{
		int temp;

		int i = 0;
		while (!in.eof())
		{
			in >> temp;
			if (temp == symbol) break;
			i++;
		}
		in.close();
	}
	else cout << "Файл не найден.";
	cout << "\nTerminated thread #" << ID;
	LeaveCriticalSection(&critSect); // ОКОНЧАНИЕ КРИТИЧЕСКОЙ СЕКЦИИ
	ExitThread(0);
}

//СОРТИРОВКА ПРОСТЫМ ДВУХПУТЕВЫМ СЛИЯНИЕМ
DWORD WINAPI simpleArraySortCS(void *)
{
	DWORD ID = GetCurrentThreadId();
	EnterCriticalSection(&critSect); //НАЧАЛО КРИТИЧЕСКОЙ СЕКЦИИ
									  // работаем с общим внешним ресурсом - глобальным массивом 

	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	int workArray[ARRAY_NUMBER];
	for (int j = 0; j < ARRAY_NUMBER; j++) workArray[j] = useArray[j];
	LeaveCriticalSection(&critSect);// ОКОНЧАНИЕ КРИТИЧЕСКОЙ СЕКЦИИ

	//СОРТИРОВКА
	simpleMergeSort(workArray, 0, ARRAY_NUMBER-1);
	
	EnterCriticalSection(&critSect); //НАЧАЛО КРИТИЧЕСКОЙ СЕКЦИИ
										  // работаем с общим внешним ресурсом - глобальным массивом 

	cout << "\n\tAcquired data access for thread #" << ID;
	for (int j = 0; j < ARRAY_NUMBER; j++) useArray[j] = workArray[j];
	cout << "\nTerminated thread #" << ID;
	LeaveCriticalSection(&critSect);// ОКОНЧАНИЕ КРИТИЧЕСКОЙ СЕКЦИИ

	ExitThread(0);
}

// СОРТИРОВКА ЕСТЕСТВЕННЫМ ДВУХПУТЕВЫМ СЛИЯНИЕМ
DWORD WINAPI naturalArraySortCS(void *)
{
	DWORD ID = GetCurrentThreadId();
	EnterCriticalSection(&critSect); //НАЧАЛО КРИТИЧЕСКОЙ СЕКЦИИ
									  // работаем с общим внешним ресурсом - глобальным массивом 

	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	int workArray[ARRAY_NUMBER];
	for (int j = 0; j < ARRAY_NUMBER; j++) workArray[j] = useArray[j];
	LeaveCriticalSection(&critSect);// ОКОНЧАНИЕ КРИТИЧЕСКОЙ СЕКЦИИ

	 //СОРТИРОВКА
	//simpleMergeSort(workArray, 0, ARRAY_NUMBER - 1);

	EnterCriticalSection(&critSect); //НАЧАЛО КРИТИЧЕСКОЙ СЕКЦИИ
										  // работаем с общим внешним ресурсом - глобальным массивом 

	cout << "\n\tAcquired data access for thread #" << ID;
	for (int j = 0; j < ARRAY_NUMBER; j++) useArray[j] = workArray[j];
	cout << "\nTerminated thread #" << ID;
	LeaveCriticalSection(&critSect);// ОКОНЧАНИЕ КРИТИЧЕСКОЙ СЕКЦИИ
	ExitThread(0);
}
//__________________________________________________________________




//__________________________________________________________________
// МЬЮТЕКС
void mutex()
{
	readFromFile("16384_50.1");

	cout << "\n\nMUTEX:\n";

	HANDLE hTHREADS[THREADS_NUMBER];
	CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//СОЗДАНИЕ ПОТОКОВ
	hTHREADS[0] = CreateThread(NULL, 0, simpleArraySortM, NULL, 0, NULL); // сортировка простым слиянием
	hTHREADS[1] = CreateThread(NULL, 0, naturalArraySortM, NULL, 0, NULL); // сортировка естественным двухпутевым слиянием
	hTHREADS[2] = CreateThread(NULL, 0, searchInArrayM, NULL, 0, NULL); // поток поиска сортировки

	if ((hTHREADS[0] == NULL) || (hTHREADS[1] == NULL) || (hTHREADS[2] == NULL)) { return; }

	WaitForMultipleObjects(NULL, hTHREADS, TRUE, INFINITE);

	writeToFile("2.1");
	//ЗАКРЫВАЕМ ПОТОКИ
	for (int i = 0; i < THREADS_NUMBER; i++) {
		CloseHandle(hTHREADS[i]);
	}
	CloseHandle(hMutex);
}

//ПОИСК ПО ФАЙЛУ
DWORD WINAPI searchInArrayM(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hMutex, INFINITE);  //ОТКРЫВАЕМ МЬЮТЕКС
	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	ifstream in("16384_50.1"); //ПРОИСХОДИТ ОБРАЩЕНИЕ К ОБЩЕМУ РЕСУРСУ - ВНЕШНЕМУ ФАЙЛУ

	if (in.is_open())
	{
		int temp;

		int i = 0;
		while (!in.eof())
		{
			in >> temp;
			if (temp == symbol) break;
			i++;
		}
		in.close();
	}
	else cout << "Файл не найден.";

	cout << "\nTerminated thread #" << ID;
	ReleaseMutex(hMutex); //ЗAКРЫВАЕМ МЬЮТЕКС
	ExitThread(0);
}

//СОРТИРОВКА ПРОСТЫМ ДВУХПУТЕВЫМ СЛИЯНИЕМ
DWORD WINAPI simpleArraySortM(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hMutex, INFINITE);  //ОТКРЫВАЕМ МЬЮТЕКС
	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	int workArray[16380];
	for (int j = 0; j < 16380; j++) workArray[j] = useArray[j];
	ReleaseMutex(hMutex); //ЗАКРЫВАЕМ МЬЮТЕКС

	//СОРТИРОВКА
	simpleMergeSort(workArray, 0, 16379);

	WaitForSingleObject(hMutex, INFINITE);  //ОТКРЫВАЕМ МЬЮТЕКС 
	cout << "\n\tAcquired data access for thread #" << ID;
	for (int j = 0; j < 16380; j++) useArray[j] = workArray[j];
	ReleaseMutex(hMutex); //ЗАКРЫВАЕМ МЬЮТЕКС


	cout << "\nTerminated thread #" << ID;
	ExitThread(0);
}

// СОРТИРОВКА ЕСТЕСТВЕННЫМ ДВУХПУТЕВЫМ СЛИЯНИЕМ
DWORD WINAPI naturalArraySortM(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hMutex, INFINITE);  //ОТКРЫВАЕМ МЬЮТЕКС
	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	int workArray[16380];
	for (int j = 0; j < 16380; j++) workArray[j] = useArray[j];
	ReleaseMutex(hMutex); //ЗАКРЫВАЕМ МЬЮТЕКС

	//СОРТИРОВКА
	simpleMergeSort(workArray, 0, 16379);

	WaitForSingleObject(hMutex, INFINITE);  //ОТКРЫВАЕМ МЬЮТЕКС
	cout << "\n\tAcquired data access for thread #" << ID;
	for (int j = 0; j < 16380; j++) useArray[j] = workArray[j];
	ReleaseMutex(hMutex); //ЗАКРЫВАЕМ МЬЮТЕКС

	cout << "\nTerminated thread #" << ID;
	ExitThread(0);
}
//____________________________________________________________



//__________________________________________________________________
// СЕМАФОР
void semaphore()
{
	readFromFile("16384_50.1");

	cout << "\n\nSEMAPHORE:\n";

	HANDLE hTHREADS[THREADS_NUMBER];
	CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//СОЗДАНИЕ ПОТОКОВ
	hTHREADS[0] = CreateThread(NULL, 0, simpleArraySortS, NULL, 0, NULL); // сортировка простым слиянием
	hTHREADS[1] = CreateThread(NULL, 0, naturalArraySortS, NULL, 0, NULL); // сортировка естественным двухпутевым слиянием
	hTHREADS[2] = CreateThread(NULL, 0, searchInArrayS, NULL, 0, NULL); // поток поиска сортировки

	if ((hTHREADS[0] == NULL) || (hTHREADS[1] == NULL) || (hTHREADS[2] == NULL)) { return; }

	WaitForMultipleObjects(NULL, hTHREADS, TRUE, INFINITE);

	writeToFile("3.1");
	//ЗАКРЫВАЕМ ПОТОКИ
	for (int i = 0; i < THREADS_NUMBER; i++) {
		CloseHandle(hTHREADS[i]);
	}

	CloseHandle(hSemaphore); //Закрываем дескриптор 
}

//ПОИСК ПО ФАЙЛУ
DWORD WINAPI searchInArrayS(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hSemaphore, INFINITE);  //Ожидаем освобождение семафора
	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	ifstream in("16384_50.1"); //ПРОИСХОДИТ ОБРАЩЕНИЕ К ОБЩЕМУ РЕСУРСУ - ВНЕШНЕМУ ФАЙЛУ

	if (in.is_open())
	{
		int temp;

		int i = 0;
		while (!in.eof())
		{
			in >> temp;
			if (temp == symbol) break;
			i++;
		}
		in.close();
	}
	else cout << "Файл не найден.";

	cout << "\nTerminated thread #" << ID;
	ReleaseSemaphore(hSemaphore, 1, NULL); // Освобождаем позицию пользования семафором, 									// увеличиваем счетчик на 1
	ExitThread(0);
}

//СОРТИРОВКА ПРОСТЫМ ДВУХПУТЕВЫМ СЛИЯНИЕМ
DWORD WINAPI simpleArraySortS(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hSemaphore, INFINITE);  //Ожидаем освобождение семафора
	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	int workArray[ARRAY_NUMBER];
	for (int j = 0; j < 16380; j++) workArray[j] = useArray[j];
	ReleaseSemaphore(hSemaphore, 1, NULL); // Освобождаем позицию пользования семафором, 									// увеличиваем счетчик на 1

	//СОРТИРОВКА
	simpleMergeSort(workArray, 0, 16379);

	WaitForSingleObject(hSemaphore, INFINITE); //Ожидаем освобождение семафора
	cout << "\n\tAcquired data access for thread #" << ID;
	for (int j = 0; j < 16380; j++) useArray[j] = workArray[j];
	ReleaseSemaphore(hSemaphore, 1, NULL); // Освобождаем позицию пользования семафором, 
										   // увеличиваем счетчик на 1

	cout << "\nTerminated thread #" << ID;
	ExitThread(0);
}

// СОРТИРОВКА ЕСТЕСТВЕННЫМ ДВУХПУТЕВЫМ СЛИЯНИЕМ
DWORD WINAPI naturalArraySortS(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hSemaphore, INFINITE);  //Ожидаем освобождение семафора

	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	int workArray[16380];
	for (int j = 0; j < 16380; j++) workArray[j] = useArray[j];
	ReleaseSemaphore(hSemaphore, 1, NULL); // Освобождаем позицию пользования семафором, 									// увеличиваем счетчик на 1
	
	//СОРТИРОВКА
	simpleMergeSort(workArray, 0, 16379);

	WaitForSingleObject(hSemaphore, INFINITE);  //Ожидаем освобождение семафора
	cout << "\n\tAcquired data access for thread #" << ID;
	for (int j = 0; j < 16380; j++) useArray[j] = workArray[j];
	ReleaseSemaphore(hSemaphore, 1, NULL); // Освобождаем позицию пользования семафором,
										   // увеличиваем счетчик на 1

	cout << "\nTerminated thread #" << ID;
	ExitThread(0);
}



//__________________________________________________________________
// СОБЫТИЯ
void events()
{
	readFromFile("16384_50.1");

	cout << "\n\nEVENTS:\n";

	HANDLE hTHREADS[THREADS_NUMBER];
	CONST HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	//СОЗДАНИЕ ПОТОКОВ
	hTHREADS[0] = CreateThread(NULL, 0, simpleArraySortE, NULL, 0, NULL); // сортировка простым слиянием
	hTHREADS[1] = CreateThread(NULL, 0, naturalArraySortE, NULL, 0, NULL); // сортировка естественным двухпутевым слиянием
	hTHREADS[2] = CreateThread(NULL, 0, searchInArrayE, NULL, 0, NULL); // поток поиска сортировки
	SetEvent(hEvent);
	if ((hTHREADS[0] == NULL) || (hTHREADS[1] == NULL) || (hTHREADS[2] == NULL)) { return; }

	WaitForMultipleObjects(NULL, hTHREADS, TRUE, INFINITE);

	writeToFile("4.1");

	//ЗАКРЫВАЕМ ПОТОКИ
	for (int i = 0; i < THREADS_NUMBER; i++) {
		CloseHandle(hTHREADS[i]);
	}

	CloseHandle(hEvent); //Закрываем дескриптор события
}

//ПОИСК ПО ФАЙЛУ
DWORD WINAPI searchInArrayE(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hEvent, INFINITE);// ОЖИДАЕМ сигнального положения флага события, 	
										  // для того чтобы продолжить выполнение программы
	ResetEvent(hEvent);  //Как только удалось получить доступ к ресурса, опускаем флаг событий, чтобы остальные потоки не имели возможности доступа к ресурсам
	
	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	ifstream in("16384_50.1"); //ПРОИСХОДИТ ОБРАЩЕНИЕ К ОБЩЕМУ РЕСУРСУ - ВНЕШНЕМУ ФАЙЛУ

	if (in.is_open())
	{
		int temp;

		int i = 0;
		while (!in.eof())
		{
			in >> temp;
			if (temp == symbol) break;
			i++;
		}
		in.close();
	}
	else cout << "Файл не найден.";


	cout << "\nTerminated thread #" << ID;
	SetEvent(hEvent);// Устанавливаем флаг вновь в сигнальное состояние после завершения 								// работы над общей частью памяти
	ExitThread(0);
}

//СОРТИРОВКА ПРОСТЫМ ДВУХПУТЕВЫМ СЛИЯНИЕМ
DWORD WINAPI simpleArraySortE(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hEvent, INFINITE);// ОЖИДАЕМ сигнального положения флага события, 	
										  // для того чтобы продолжить выполнение программы
	ResetEvent(hEvent);  //Как только удалось получить доступ к ресурса, опускаем флаг событий, чтобы остальные потоки не имели возможности доступа к ресурсам

	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	
	int workArray[16380];
	for (int j = 0; j < 16380; j++) workArray[j] = useArray[j];
	SetEvent(hEvent); // Устанавливаем флаг вновь в сигнальное состояние после завершения 								// работы над общей частью памяти
					 //СОРТИРОВКА
	simpleMergeSort(workArray, 0, 16379);

	WaitForSingleObject(hEvent, INFINITE);// ОЖИДАЕМ сигнального положения флага события,
										  // для того чтобы продолжить выполнение программы

	ResetEvent(hEvent);  //Как только удалось получить доступ к ресурса, опускаем флаг событий, чтобы остальные потоки не имели возможности доступа к ресурсам
	cout << "\n\tAcquired data access for thread #" << ID;
	
	for (int j = 0; j < 16380; j++) useArray[j] = workArray[j];

	cout << "\nTerminated thread #" << ID;
	SetEvent(hEvent); // Устанавливаем флаг вновь в сигнальное состояние после завершения 								// работы над общей частью памяти
	ExitThread(0);
}

// NATURAL DOUBLE WAY MERGE SORT
DWORD WINAPI naturalArraySortE(void *)
{
	DWORD ID = GetCurrentThreadId();

	WaitForSingleObject(hEvent, INFINITE);// ОЖИДАЕМ сигнального положения флага события, 		
										  // для того чтобы продолжить выполнение программы
	ResetEvent(hEvent);  //Как только удалось получить доступ к ресурса, опускаем флаг событий, чтобы остальные потоки не имели возможности доступа к ресурсам

	cout << "\nLaucned thread #" << ID;
	cout << "\n\tAcquired data access for thread #" << ID;
	
	int workArray[16380];
	for (int j = 0; j < 16380; j++) workArray[j] = useArray[j];
	SetEvent(hEvent); // Устанавливаем флаг вновь в сигнальное состояние после завершения 								// работы над общей частью памяти
					 //СОРТИРОВКА
	simpleMergeSort(workArray, 0, 16379);

	WaitForSingleObject(hEvent, INFINITE);// ОЖИДАЕМ сигнального положения флага события, 		
										  // для того чтобы продолжить выполнение программы
	ResetEvent(hEvent);  //Как только удалось получить доступ к ресурса, опускаем флаг событий, чтобы остальные потоки не имели возможности доступа к ресурсам
	cout << "\n\tAcquired data access for thread #" << ID;

	for (int j = 0; j < 16380; j++) useArray[j] = workArray[j];

	cout << "\nTerminated thread #" << ID;
	SetEvent(hEvent); // Устанавливаем флаг вновь в сигнальное состояние после завершения 								// работы над общей частью памяти
	ExitThread(0);
}

void putHorizontalLine(int size, int type)
{
	int i;
	switch (type)
	{
		case 0:	cout << (char)195; break;
		case 1:	cout << (char)218; break;
		case 2:	cout << (char)192; break;
		default: break;
	}

	for (i = 1; i < size - 1; i++)
	{
		if (i == 20)
		{
			switch (type)
			{
				case 0: cout << (char)197; break;
				case 1: cout << (char)194; break;
				case 2: cout << (char)193; break;
			}
		}
		else cout << (char)196;
	}

	switch (type)
	{
		case 0:	cout << (char)180; break;
		case 1:	cout << (char)191; break;
		case 2:	cout << (char)217; break;
		default: break;
	}

	cout << endl;
}