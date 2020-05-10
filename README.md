# Corrosion
Rust ASM Compiler  made with C++

## Build
First of all clone this repository 
```
git clone --recurse-submodules https://github.com/alohaeee/Corrosion.git
```
Then you must [install spdlog](https://github.com/gabime/spdlog#install). 
```
cd thirdparty && cd spdlog && mkdir build && cd build
cmake .. && make -j
```
And then build project from root.
```
cd build 
cmake --build ..
```
## Third party
[spdlog](https://github.com/gabime/spdlog)
