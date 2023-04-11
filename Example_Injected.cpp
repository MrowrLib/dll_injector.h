#include <DLL_Injection/Injected_DLL.h>
#include <Logging.h>

SetLogFilePath("C:/temp/Example Injected DLL.log");

int hello = 69;

DLL_Main {
    Log("Hello from injected DLL!");

    uintptr_t addressOfHello = reinterpret_cast<uintptr_t>(&hello);
    Log("The address of an int is: {:x}", addressOfHello);
}
