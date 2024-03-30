#pragma once

#include <Windows.h>
// ^ require Windows.h first

#include <TlHelp32.h>
#include <_Log_.h>
#include <tchar.h>

#include <filesystem>
#include <string>

// TODO: make it one function called like GetProcessIdByName() which returns a std::promise<DWORD>
namespace DLL_Injection {

    namespace ProcessHelpers {

        DWORD GetProcId(const std::string& exeName) {
            std::basic_string<TCHAR> exeNameT;
#ifdef UNICODE
            int exeNameLength = MultiByteToWideChar(CP_UTF8, 0, exeName.c_str(), -1, nullptr, 0);
            exeNameT.resize(exeNameLength - 1);
            MultiByteToWideChar(CP_UTF8, 0, exeName.c_str(), -1, &exeNameT[0], exeNameLength);
#else
            exeNameT = exeName;
#endif
            DWORD  procId = 0;
            HANDLE hSnap  = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            if (hSnap != INVALID_HANDLE_VALUE) {
                PROCESSENTRY32 procEntry;
                procEntry.dwSize = sizeof(procEntry);
                if (Process32First(hSnap, &procEntry)) {
                    do {
                        if (!_tcsicmp(procEntry.szExeFile, exeNameT.c_str())) {
                            procId = procEntry.th32ProcessID;
                            break;
                        }
                    } while (Process32Next(hSnap, &procEntry));
                }
            }
            CloseHandle(hSnap);
            return procId;
        }

        DWORD WaitForProcId(const std::string& exeName, DWORD intervalMs = 30) {
            DWORD procId = 0;
            while (!procId) {
                procId = GetProcId(exeName);
                Sleep(intervalMs);
            }
            return procId;
        }

        bool IsProcessRunning(const std::string& exeName) { return GetProcId(exeName) != 0; }
    }

    namespace Techniques {

        bool InjectDLL_using_CreateRemoteThread_LoadLibraryA(
            const std::string& exeName, const std::string& dllPath, bool waitForProcess = true
        ) {
            DWORD procID;
            if (waitForProcess) procID = ProcessHelpers::WaitForProcId(exeName);
            else procID = ProcessHelpers::GetProcId(exeName);

            if (!procID) {
                _Log_("Process not found: {}", exeName);
                return false;
            }
            _Log_("Injecting DLL '{}' into process '{}' (PID: {})", dllPath, exeName, procID);

            HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);
            if (hProc && hProc != INVALID_HANDLE_VALUE) {
                void* loc =
                    VirtualAllocEx(hProc, 0, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
                WriteProcessMemory(hProc, loc, dllPath.c_str(), dllPath.size() + 1, 0);
                HANDLE hThread = CreateRemoteThread(
                    hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0
                );
                if (hThread) CloseHandle(hThread);
            }
            return true;
        }
    }

    bool InjectDLL(
        const std::string& exeName, const std::string& dllPath, bool waitForProcess = false
    ) {
        if (!std::filesystem::exists(dllPath)) {
            _Log_("DLL does not exist: {}", dllPath);
            return false;
        }
        auto absoluteDllPath = std::filesystem::absolute(dllPath).string();
        return Techniques::InjectDLL_using_CreateRemoteThread_LoadLibraryA(
            exeName, absoluteDllPath, waitForProcess
        );
    }
}
