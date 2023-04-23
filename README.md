# `#include <dll_injection>`

```cpp
#include <dll_injection>

void Example() {
    InjectDLL("app.exe", "library.dll");
}
```

## What?

Convenient helper to inject a DLL into a running Windows process.

## Installation

### xmake

#### `xmake.lua`

```lua
add_repositories("MrowrLib https://github.com/MrowrLib/Packages.git")
add_requires("dll_injection")

target("Example")
    add_packages("dll_injection")
```

### vcpkg

#### `CMakeLists.txt`

```cmake
add_executable(Example main.cpp)

# Find dll_injection and link it to your target
find_package(dll_injection CONFIG REQUIRED)
target_link_libraries(Example PRIVATE dll_injection::dll_injection)

# If you want logs
find_package(spdlog CONFIG REQUIRED)
target_link_libraries(Example PRIVATE spdlog::spdlog)
```

#### `vcpkg.json`

```json
{
    "dependencies": ["mrowr-dll-injection"]
}
```

If you want `dll_injection` to log its progress:

```json
{
    "dependencies": ["mrowr-dll-injection", "spdlog"]
}
```

#### `vcpkg-configuration.json`

```json
{
    "default-registry": {
        "kind": "git",
        "repository": "https://github.com/microsoft/vcpkg.git",
        "baseline": "95252eadd63118201b0d0df0b4360fa613f0de84"
    },
    "registries": [
        {
            "kind": "git",
            "repository": "https://github.com/MrowrLib/Packages.git",
            "baseline": "2091c425976da4956a0f61469d3679a4a876b809",
            "packages": ["mrowr-dll-injection", "mrowr-log"]
        }
    ]
}
```

> _Update the default-registry baseline to the latest commit from https://github.com/microsoft/vcpkg_  
> _Update the MrowrLib/Packages baseline to the latest commit from https://github.com/MrowrLib/Packages_  
> _**Don't forget to include mrowr-log** (dependency of dll_injection)_

## Why?

I was frequently injecting DLLs into really old games on Windows.

## How?

```cpp
#include <dll_injection>
#include <iostream>

void Example() {
    // If the app.exe process is not found (its not running), this returns false immediately:
    if (InjectDLL("app.exe", "library.dll"))
        std::cout << "Injected library.dll into app.exe" << std::endl;

    // If you want to block and wait for the process to start, set the wait parameter to true:
    InjectDLL("app.exe", "library.dll", true);
}
```

### How does it work?

- Finds the process ID of the process with the given name
  - By default, will wait for the process to start if it is not running
- Opens a handle to the process
- Allocates memory in the process to store the path to the DLL
- Writes the path to the DLL into the process's memory
- Creates a remote thread in the process to load the DLL
- Closes the handle to the process

## License

Use however, no attribution required.

```
BSD Zero Clause License (SPDX: 0BSD)

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.
```
