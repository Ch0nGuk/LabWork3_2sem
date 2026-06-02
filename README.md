# PiecewiseLab

Лабораторная работа по реализации целевого АТД "кусочная функция" на C++.

Проект позволяет создавать кусочно-заданные функции, доопределять и переопределять их на отрезках, вычислять значения в точках, проверять непрерывность и монотонность, строить графики и создавать кусочно-квадратичную аппроксимацию.

## Возможности

- Создание нескольких кусочных функций.
- Задание функции на выбранном отрезке.
- Переопределение уже заданных участков.
- Поддержка базовых функций:
  - `5x + 7`
  - `exp(x)`
  - `sin(x)`
  - `x^4 - 3x^3 + 7x`
  - `1 / x^2`
- Вычисление значения кусочной функции в точке.
- Проверка непрерывности на отрезке.
- Проверка монотонности на отрезке.
- Построение графика через ImGui, ImPlot и GLFW.
- Построение кусочно-квадратичной аппроксимации с совпадением первой производной на стыках.
- Встроенные тесты основных операций.

## Аппроксимация

Аппроксимация строит новую кусочную функцию по значениям выбранной исходной функции на равномерной сетке точек.

На каждом интервале `[x_i, x_{i+1}]` создается квадратичный кусок:

```text
S_i(x) = y_i + s_i * (x - x_i) + a_i * (x - x_i)^2
```

Коэффициенты подбираются так, чтобы:

- кусок проходил через соседние узлы;
- значения на стыках совпадали;
- первая производная на стыках совпадала.

Метод рассчитан на непрерывные функции. Разрывные функции следует аппроксимировать отдельно на каждом интервале непрерывности.

## Зависимости

Для сборки графической части нужны development-пакеты с заголовками и библиотеками. CMake ищет зависимости через `find_package(...)` или `pkg-config`:

- OpenGL
- GLFW 3
- ImGui с GLFW/OpenGL3 backend-заголовками
- Threads
- pkg-config для fallback-поиска пакетов

ImPlot сначала ищется как системный пакет через CMake/pkg-config. Если системный ImPlot не установлен, CMake использует локальный fallback `External/implot`.

Fallback зафиксирован на версии `v0.15`: она совместима с системным `libimgui-dev` версии `1.90.1`. Более новые версии ImPlot могут использовать `ImDrawList::AddConcavePolyFilled`, которого нет в ImGui `1.90.1`.

## Windows

Требуется:

- CMake
- Visual Studio 2022 или Visual Studio Build Tools с компонентом C++
- development-пакеты GLFW, ImGui, OpenGL и, желательно, ImPlot, доступные для `find_package`/pkg-config

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
- development-пакеты GLFW, ImGui и OpenGL
- pkg-config
- ImPlot development package, если он доступен в репозиториях дистрибутива; иначе используется fallback `External/implot`

Fedora/RHEL-подобные системы:

```bash
sudo dnf install cmake gcc-c++ glfw-devel mesa-libGL-devel imgui-devel implot-devel pkgconf-pkg-config
```

Debian/Ubuntu:

```bash
sudo apt update
sudo apt install build-essential cmake libglfw3-dev libimgui-dev libgl1-mesa-dev pkg-config
```


Сборка

```bash
cmake -S . -B build
cmake --build build -j"$(nproc)"
```

Запуск

```bash
./build/LabApp
```
