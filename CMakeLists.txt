cmake_minimum_required(VERSION 3.10)
project(PiecewiseLab)

set(CMAKE_CXX_STANDARD 17)

# Ищем GLFW (уже установлен в системе)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)


# Подключаем пути к заголовкам
include_directories(. External/imgui External/imgui/backends External/implot)

# Собираем список всех исходников
set(SOURCES
    ui/continuity_ui.cpp
    ui/create_function_ui.cpp
    ui/define_piece_ui.cpp
    ui/evaluate_ui.cpp
    ui/monotonicity_ui.cpp
    ui/piecewise_ui_utils.cpp
    ui/print_functions_ui.cpp
    ui/run_tests_ui.cpp
    ui/ui.cpp
    ui/plot_ui.cpp
    main.cpp
    tests.cpp
    
    # Исходники ImGui
    External/imgui/imgui.cpp
    External/imgui/imgui_draw.cpp
    External/imgui/imgui_widgets.cpp
    External/imgui/imgui_tables.cpp
    External/imgui/backends/imgui_impl_glfw.cpp
    External/imgui/backends/imgui_impl_opengl3.cpp
    
    # Исходники ImPlot
    External/implot/implot.cpp
    External/implot/implot_items.cpp
)

add_executable(LabApp ${SOURCES})

# Линковка
target_link_libraries(LabApp 
    glfw 
    ${OPENGL_LIBRARIES} 
    Threads::Threads
    ${CMAKE_DL_LIBS}
)