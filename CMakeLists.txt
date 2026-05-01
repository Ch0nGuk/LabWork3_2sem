cmake_minimum_required(VERSION 3.10)
project(PiecewiseLab)

set(CMAKE_CXX_STANDARD 17)

# Ищем GLFW (уже установлен в системе)
find_package(glfw3 REQUIRED)

# Подключаем пути к заголовкам
include_directories(headers external/imgui external/imgui/backends external/implot)

# Собираем список всех исходников
set(SOURCES
    main.cpp
    ui.cpp
    piecewise_ui_utils.cpp
    
    # Добавьте сюда все остальные ваши cpp файлы (create_function_ui.cpp и т.д.)
    
    # Исходники ImGui
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
    
    # Исходники ImPlot
    external/implot/implot.cpp
    external/implot/implot_items.cpp
)

add_executable(LabApp ${SOURCES})

# Линковка
target_link_libraries(LabApp glfw OpenGL::GL)