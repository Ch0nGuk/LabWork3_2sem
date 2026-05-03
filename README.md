### Linux

1) **Установка зависимостей**

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev
```

2) **Сборка проекта** (из корня папки проекта):

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

3) **Запуск**

```bash
./LabApp
```

---

### Windows

1) **В корне проекта**

```cmd
mkdir build
cd build
```

2) **Генерация файлов сборки**

```cmd
cmake ..
```

3) **Сборка**

```cmd
cmake --build . --config Release
```

4) **Запуск**

```cmd
.\Release\LabApp.exe
```
