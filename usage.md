# ECS usage

## Building

*This project requires a C++ compiler*

1. Clone the repository:
```bash
git clone https://github.com/lammmab/emulator-cloud-saves.git
cd emulator-cloud-saves
```
2. Create the build directory:
```bash
mkdir build
```

### Windows (MSYS2 UCRT64)
Build:
```bash
g++ src/main.cpp -g -o build/main.exe
```

### Linux / MacOS
Build:
```bash
g++ src/main.cpp -g -o build/main
```