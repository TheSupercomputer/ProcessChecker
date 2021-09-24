# ProcessChecker
This is a little helper for checking wheter or not a programm is running.
It works with linux and Windows 64 and 32 Bit.
The Process checker provides an easy to use platform independed interafce.
The default constructor will add the most common broadcasting softwares for each plattform per default to check for.
A main function is provided to give you a quick start.

## Examples

```cpp
#include <string>
#include <vector>

#include "processchecker.hpp"

int main() {
    // Default checking for OBS, Streamlabs OBS, XSplit and vokoscreenNG on Windows
    // Default checking for OBS, vokoscreenNG and ffscreencast on Linux
    processchecker ProcessChecker;
    
    if(ProcessChecker.is_running()) {
        // Do something
        for(auto& entry : ProcessChecker.what_software()) {
            if(entry == "/usr/bin/obs") {
                // Special handling for OBS on Linux
            }
        }
    }
    
    // The provided overloads will bypass the internal SoftwareToCheck variable
    // usefull for quick checking of different softwares in special cases without the need
    // to create new objects of type processchecker
    if(ProcessChecker.is_running("cheat.exe")) {
        // Do something fancy angainst the cheater
    }
    
    // You can also groupe programs to check whether or not they are running.
    std::vector<std::string> bad_programs = {"cheater.exe", "new_cheater.exe"};
    if(ProcessChecker.is_running(bad_programs)) {
        // Have fun against cheaters
    }
    
    //Creating a processchecker just to check for cheating
    processchecker CheatProcesses(bad_programs);
    if(CheatProcesses.is_running()) {
        // Again get creative against cheaters
    }
    
    // To check for additional running Programms just add them
    // they will get checked when calling "is_running()"
    CheatProcesses.add_software("new_cheat_plus.exe");
}
```
