# Spoon

## Compiling
### Windows

MSVC 2019+ Required

#### Prerequisites
Most libraries are included precompiled in the source tree /Lib except for PhysX due to its size.

Pull PhysX master tree from https://github.com/NVIDIAGameWorks/PhysX and copy as /Lib/PhysX-4.1. Enter the PhysX-4.1/physx folder and run the generate_projects.bat (requires CMake). Open the .sln from the compiler folder and build the solution. The samples and snippet projects are not required.


### Linux
TODO
