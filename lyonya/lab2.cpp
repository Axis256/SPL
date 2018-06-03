#include <windows.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <fstream>

void printSpace() {
	int i;
	printf("%c", 195);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 197);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 197);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 197);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c\n", 180);
}

void sortArray(int *myArray, int size) {
	int min;
	for (int i = 0; i < size - 1; i++) {
		min = myArray[i];
		for (int y = i + 1; y < size; y++) {
			if (min > myArray[y]) {
				min = myArray[y];
				myArray[y] = myArray[i];
				myArray[i] = min;
			}
		}
	}
}

void printHead() {
	int i;
	printf("%c", 218);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 194);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 194);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 194);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c\n", 191);


	printf("%c", 179);
	printf("     Size      ");
	printf("%c", 179);
	printf("     Heaps     ");
	printf("%c", 179);
	printf(" Mapping Files ");
	printf("%c", 179);
	printf(" Base indexing ");
	printf("%c\n", 179);

	printf("%c", 195);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 197);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 197);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 197);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c\n", 180);

}

void printString(int size, int c, int windows, int copyFile) {
	printf("%c", 179);
	printf("%15d", size);
	printf("%c", 179);
	printf("%15d", c);
	printf("%c", 179);
	printf("%15d", windows);
	printf("%c", 179);
	printf("%15d", copyFile);
	printf("%c\n", 179);
}

void printFooter() {
	int i;
	printf("%c", 192);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 193);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 193);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c", 193);
	for (i = 0; i < 15; i++) printf("%c", 196);
	printf("%c\n", 217);
}


int main() {

	int count, startTime, endTime, cuchTime, fileTime, baseTime;
	char* files[] = { const_cast<char *>("2048_000.1"), const_cast<char *>("4096_075.2"), const_cast<char *>("8192_000.1"), const_cast<char *>("16384_50.1") };
	HANDLE hFile;

	printHead();

	for (int j = 0; j < 4; j++) {
		std::ifstream in(files[j]);
		if (in.is_open()) {
			count = 0;
			int temp;

			while (!in.eof()) {
				in >> temp;
				count++;
			}
			count--;
			in.close();
		}

		hFile = (HANDLE)CreateFileA((LPCSTR)files[j], GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE) {
			std::cerr << "fileMappingCreate - CreateFile failed, fname1 = " << files[j] << std::endl;
			return 0;
		}

		HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY,
			0, 0, nullptr);
		if (hMapping == nullptr) {
			std::cerr << "fileMappingCreate - CreateFileMapping failed, fname = "
				<< files[j] << std::endl;
			CloseHandle(hFile);
			return 0;
		}

		DWORD dwFileSize = GetFileSize(hFile, nullptr);

		if (dwFileSize == INVALID_FILE_SIZE) {
			std::cerr << "fileMappingCreate - GetFileSize failed, fname = "
				<< files[j] << std::endl;
			CloseHandle(hFile);
			return 0;
		}

		auto *dataPtr = (char *)MapViewOfFile(hMapping,
			FILE_MAP_READ,
			0,
			0,
			dwFileSize);
		if (dataPtr == nullptr) {
			std::cerr << "fileMappingCreate - MapViewOfFile failed, fname = "
				<< files[j] << std::endl;
			CloseHandle(hMapping);
			CloseHandle(hFile);
			return 0;
		}
		startTime = clock();

		auto *myArray = new char[dwFileSize];

		for (int i = 0; i < dwFileSize; i++) {

			myArray[i] = *(dataPtr + i);
		}

		char *pEnd;
		long int t2;
		auto *array = new int[count];
		long int t = strtol(dataPtr, &pEnd, 10);
		array[0] = t;
		for (int i = 1; i < count; i++) {
			t2 = strtol(pEnd, &pEnd, 10);
			array[i] = t2;
		}

		sortArray(array, count);

		UnmapViewOfFile(dataPtr);
		CloseHandle(hMapping);
		CloseHandle(hFile);

		endTime = clock();

		fileTime = endTime - startTime;

		startTime = clock();

		hFile = CreateFileA((LPCSTR)files[j], GENERIC_READ, 0, nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE) {
			std::cerr << "fileMappingCreate - CreateFile failed, fname = " << files[j] << std::endl;
			return 0;
		}

		hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY,
			0, 0, nullptr);
		if (hMapping == nullptr) {
			std::cerr << "fileMappingCreate - CreateFileMapping failed, fname = "
				<< files[j] << std::endl;
			CloseHandle(hFile);
			return 0;
		}

		dwFileSize = GetFileSize(hFile, nullptr);
		if (dwFileSize == INVALID_FILE_SIZE) {
			std::cerr << "fileMappingCreate - GetFileSize failed, fname = "
				<< files[j] << std::endl;
			CloseHandle(hFile);
			return 0;
		}

		dataPtr = (char *)MapViewOfFile(hMapping,
			FILE_MAP_READ,
			0,
			0,
			dwFileSize);
		if (dataPtr == nullptr) {
			std::cerr << "fileMappingCreate - MapViewOfFile failed, fname = "
				<< files[j] << std::endl;
			CloseHandle(hMapping);
			CloseHandle(hFile);
			return 0;
		}


		pEnd;
		t = strtol(dataPtr, &pEnd, 10);
		array[0] = t;
		for (int i = 1; i < count; i++) {
			t2 = strtol(pEnd, &pEnd, 10);
			array[i] = t2;

		}

		sortArray(array, count);
		endTime = clock();

		UnmapViewOfFile(dataPtr);
		CloseHandle(hMapping);
		CloseHandle(hFile);

		baseTime = endTime - startTime;

		std::ifstream in3(files[j]);

		if (in3.is_open()) {

			count = 0;
			int temp;
			startTime = clock();
			while (!in3.eof()) {
				in3 >> temp;
				count++;
			}
			count--;
			auto *myArray = new int[count];
			in3.close();

			std::ifstream in(files[j]);

			for (int i = 0; i < count; i++) {
				in >> temp;
				myArray[i] = temp;
			}

			sortArray(myArray, count);
			sortArray(myArray, count);
			sortArray(myArray, count);
			sortArray(myArray, count);

			endTime = clock();
			cuchTime = endTime - startTime;

			in.close();
		}

		printString(count, cuchTime, fileTime, baseTime);
		if (j != 3) printSpace();
	}
	printFooter();

	system("PAUSE");
	return 0;
}