#include <windows.h>
#include <iostream>
#include <psapi.h>

// Getting Module Base Address of EXEs

// Using EnumProcessModules (For External Processes)
DWORD GetProcessIdByName(const wchar_t* processName) {
	DWORD processes[1024], processCount;
	if (!EnumProcesses(processes, sizeof(processes), &processCount)) return 0;

	processCount /= sizeof(DWORD);
	for (size_t i = 0; i < processCount; i++) {
		if (processes[i] == 0) continue;

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processes[i]);
		if (hProcess) {
			WCHAR exeName[MAX_PATH];
			if (GetModuleBaseNameW(hProcess, NULL, exeName, MAX_PATH) > 0) { // An error will occur: "of type WCHAR incompatible with of type LPSTR, so change character set settings in VS to use Unicode instead of Multi-Byte
				if (_wcsicmp(exeName, processName) == 0) {
					CloseHandle(hProcess);
					return processes[i];
				}
			}
			CloseHandle(hProcess);
		}
	}
	return 0;
}

HMODULE GetBaseAddress(DWORD processID) {
	HMODULE hModules[1024];
	DWORD cbNeeded;
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

	if (hProcess) {
		if (EnumProcessModules(hProcess, hModules, sizeof(hModules), &cbNeeded)) {
			CloseHandle(hProcess);
			return hModules[0]; // First module is the EXE itself.
		}
		CloseHandle(hProcess);
	}
	return NULL;
}

int main() {
	HMODULE hModule = GetModuleHandle(NULL); // Null returns base address of this EXE
	std::cout << "Base Address " << hModule << std::endl; // Con: Won't work for external processes

	DWORD pid = GetProcessIdByName(L"ac_client.exe"); // Change to target process
	if (pid == 0) {
		std::cout << "Process not found!" << std::endl;
		return 1;
	}

	HMODULE baseAddress = GetBaseAddress(pid);
	std::cout << "Base Address: " << baseAddress << std::endl;

	return 0;
}