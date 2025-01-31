#include <windows.h>
#include <iostream>

int main() {
	HMODULE hModule = GetModuleHandle(NULL); // Null returns base address of this EXE
	std::cout << "Base Address " << hModule << std::endl;

	return 0;
}
