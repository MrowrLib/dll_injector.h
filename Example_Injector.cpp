#include <dll_injection.h>

#include <iostream>

_LogToFile_("DLL Injector.log");

int main(int argc, char** argv) {
    std::string processName{"LogExpert.exe"};
    std::string dllPath{"Example_Injected.dll"};

    if (argc == 3) {
        processName = argv[1];
        dllPath     = argv[2];
    }

    std::cout << "Injecting " << dllPath << " into " << processName << " . . .\n";

    InjectDLL(processName, dllPath, false);

    std::cout << "Injection complete!\n";
}
