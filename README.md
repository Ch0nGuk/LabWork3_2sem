# PiecewiseLab

Проект собирается с локальными копиями UI-зависимостей:

- `External/imgui`
- `External/implot`
- `External/glfw`

Системный пакет GLFW устанавливать не нужно.

## Windows

Требуется:

- CMake
- Visual Studio 2022 или Visual Studio Build Tools с компонентом C++

Сборка из корня проекта:

```cmd
cmake -S . -B build
cmake --build build --config Release
build\Release\LabApp.exe
```

## Linux

Требуется:

- CMake
- C++17 compiler
- системные OpenGL/X11 development libraries

Пример для Ubuntu/Debian:

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev

cmake -S . -B build
cmake --build build -j"$(nproc)"
./build/LabApp
```
