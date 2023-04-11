#include <DLL_Injection/DLL_Injector.h>

#include <iostream>

int main(int argc, char** argv) {
    auto processName = argv[1];
    auto dllPath     = argv[2];

    std::cout << "Injecting " << dllPath << " into " << processName << " . . .\n";

    DLL_Injector::InjectDLL(processName, dllPath, false);

    std::cout << "Injection complete!\n";
}
