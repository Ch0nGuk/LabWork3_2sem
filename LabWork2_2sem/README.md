# LabWork2_2sem

Лабораторная работа по линейным структурам данных на C++.

## Что есть в проекте

- `DynamicArray<T>`
- `LinkedList<T>`
- `Sequence<T>`
- `ArraySequence<T>`
- `MutableArraySequence<T>`
- `ImmutableArraySequence<T>`
- `ListSequence<T>`
- `IEnumerator<T>`
- внешний generic `Map`
- внешний generic `Zip`
- внешний generic `Unzip`
- консольный UI
- тесты

Сейчас пользовательский интерфейс работает в основном с `Sequence<int>`, а также умеет показывать результаты `Map` в `Sequence<double>` и `Zip` в `Sequence<std::pair<int, int>>`.

## Структура

- ядро структур данных и sequence-архитектуры лежит в корне проекта
- UI вынесен в папку `ui/`
- тесты находятся в `tests.cpp`

## Сборка и запуск

Все команды ниже нужно выполнять из корня проекта.

### Windows

Открой `x64 Native Tools Command Prompt for VS 2022` или подними окружение MSVC вручную:

```bat
"C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
```

Сборка:

```bat
cl /nologo /EHsc ui\append_ui.cpp ui\concat_ui.cpp ui\create_sequence_ui.cpp ui\insert_at_ui.cpp main.cpp ui\map_ui.cpp ui\prepend_ui.cpp ui\print_sequences_ui.cpp ui\reduce_ui.cpp ui\run_tests_ui.cpp ui\sequence_ui_utils.cpp ui\slice_ui.cpp ui\subsequence_ui.cpp tests.cpp ui\ui.cpp ui\where_ui.cpp ui\zip_ui.cpp ui\unzip_ui.cpp /Fe:lab_app.exe
```

Запуск из `cmd`:

```bat
lab_app.exe
```

Запуск из PowerShell:

```powershell
.\lab_app.exe
```

### Linux

В Linux используется `g++` и `Makefile`.

Сборка:

```bash
make
```

Запуск:

```bash
./lab_app
```

Очистка объектных файлов и бинарника:

```bash
make clean
```

## Возможности UI

Через меню можно:

- создавать последовательности
- печатать сохраненные последовательности
- выполнять `Append`
- выполнять `Prepend`
- выполнять `InsertAt`
- получать `GetSubsequence`
- выполнять `Concat`
- выполнять `Map`
- выполнять `Where`
- выполнять `Reduce`
- выполнять `Slice`
- выполнять `Zip`
- выполнять `Unzip`
- запускать тесты

## Тесты

Тесты запускаются через пункт меню `Run tests`.

Они покрывают:

- `DynamicArray`
- `LinkedList`
- `MutableArraySequence`
- `ImmutableArraySequence`
- `ListSequence`
- `IEnumerator`
- `Map`
- `Zip`
- `Unzip`
