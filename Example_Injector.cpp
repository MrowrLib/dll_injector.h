#include <dll_injection>
#include <iostream>


int main(int argc, char** argv) {
    auto processName = argv[1];
    auto dllPath     = argv[2];

    std::cout << "Injecting " << dllPath << " into " << processName << " . . .\n";

    InjectDLL(processName, dllPath, false);

    std::cout << "Injection complete!\n";
}
