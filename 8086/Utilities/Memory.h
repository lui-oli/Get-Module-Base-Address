#pragma once

#include <Windows.h>
#include <Tlhelp32.h>
#include <cstdint>
#include <string_view>


class Memory {
	private:
		std::uintptr_t processID;
		void* processHandle = nullptr;
	public:
		Memory(const std::string_view processName) noexcept	{
			::PROCESSENTRY32 entry = { };
			entry.dwSize = sizeof(::PROCESSENTRY32);

			const auto processSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

			while (::Process32Next(processSnapshot, &entry)) {
				if (!processName.compare(entry.szExeFile))
				{
					processID = entry.th32ProcessID;;
				}
			}
		}

};

// Create a snapshot of all the running process on our computer
// Automatically find the right game process
// Get full modifying access to the game
// Make getModuleBaseAddress function

// ReadProcessMemory
// WriteProcessMemory





