#include <windows.h>
#include <iostream>
#include <conio.h>
#include <TlHelp32.h>

using namespace std;

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RU");

	STARTUPINFO StartupInfo[3];
	PROCESS_INFORMATION ProcInfo[3];
	LPSTR CommandLine[3] = {	(LPSTR)"SortAndSearchProcess.exe 530 16384_50.1", 
								(LPSTR)"SortAndSearchProcess.exe 65 4096_075.2",  
								(LPSTR)"SortAndSearchProcess.exe 133 8192_000.1" 
	};

	std::cout << "lab4 ID - " << GetCurrentProcessId() << "\n";
	for (size_t i = 0; i < 3; i++)
	{
		ZeroMemory(&StartupInfo[i], sizeof(StartupInfo[i]));
		StartupInfo[i].cb = sizeof(StartupInfo[i]);
		ZeroMemory(&ProcInfo[i], sizeof(ProcInfo[i]));
		std::cout << "\n" << i + 1 << "-й  процесс\n";

		if (!CreateProcess(NULL, CommandLine[i], NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo[i], &ProcInfo[i]))
			std::cout << "CreateProcess failed.\n";

		// Ждать окончания дочернего процесса
		Sleep(5000);
	}
	
	PROCESSENTRY32 Process;
	HANDLE hProcess;
	Process.dwSize = sizeof(PROCESSENTRY32);

	hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	Process32First(hProcess, &Process);

	int countProc = 0;
	while (Process32Next(hProcess, &Process))
	{
		Process32Next(hProcess, &Process);
		countProc++;
	}
	std::cout << "\nКоличество открытых дескрипторов: " << countProc << "\n";
	system("PAUSE");

	

	return 0;
}
