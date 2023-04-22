#include <Windows.h>

#include <_Log_>

_LogToFile_("C:/temp/Example Injected DLL.log");

int hello = 69;

// This function is called when the DLL is loaded
int DllMain(HINSTANCE dll, DWORD reason, LPVOID _) {
    if (reason == DLL_PROCESS_ATTACH) {
        _Log_("Hello from injected DLL!");

        uintptr_t addressOfHello = reinterpret_cast<uintptr_t>(&hello);
        _Log_("The address of an int is: {:x}", addressOfHello);
    }
    return TRUE;
}
