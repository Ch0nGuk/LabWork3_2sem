cmake_minimum_required(VERSION 3.10)
project(PiecewiseLab)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(glfw3 REQUIRED)
find_package(PkgConfig QUIET)

find_package(imgui QUIET)

set(LAB_IMGUI_TARGET "")
if(TARGET imgui::imgui)
    set(LAB_IMGUI_TARGET imgui::imgui)
elseif(TARGET imgui)
    set(LAB_IMGUI_TARGET imgui)
elseif(TARGET ImGui::ImGui)
    set(LAB_IMGUI_TARGET ImGui::ImGui)
endif()

if(NOT LAB_IMGUI_TARGET)
    find_package(ImGui QUIET)
    if(TARGET imgui::imgui)
        set(LAB_IMGUI_TARGET imgui::imgui)
    elseif(TARGET imgui)
        set(LAB_IMGUI_TARGET imgui)
    elseif(TARGET ImGui::ImGui)
        set(LAB_IMGUI_TARGET ImGui::ImGui)
    endif()
endif()

if(NOT LAB_IMGUI_TARGET)
    if(NOT PkgConfig_FOUND)
        find_package(PkgConfig REQUIRED)
    endif()

    pkg_check_modules(IMGUI QUIET IMPORTED_TARGET imgui)
    if(TARGET PkgConfig::IMGUI)
        set(LAB_IMGUI_TARGET PkgConfig::IMGUI)
    endif()
endif()

if(NOT LAB_IMGUI_TARGET)
    message(FATAL_ERROR
        "ImGui development package was not found. Install imgui-devel/libimgui-dev "
        "or provide a CMake target imgui::imgui, imgui, ImGui::ImGui, or pkg-config module imgui.")
endif()

find_package(implot QUIET)

set(LAB_IMPLOT_TARGET "")
if(TARGET implot::implot)
    set(LAB_IMPLOT_TARGET implot::implot)
elseif(TARGET implot)
    set(LAB_IMPLOT_TARGET implot)
elseif(TARGET ImPlot::ImPlot)
    set(LAB_IMPLOT_TARGET ImPlot::ImPlot)
endif()

if(NOT LAB_IMPLOT_TARGET)
    find_package(ImPlot QUIET)
    if(TARGET implot::implot)
        set(LAB_IMPLOT_TARGET implot::implot)
    elseif(TARGET implot)
        set(LAB_IMPLOT_TARGET implot)
    elseif(TARGET ImPlot::ImPlot)
        set(LAB_IMPLOT_TARGET ImPlot::ImPlot)
    endif()
endif()

if(NOT LAB_IMPLOT_TARGET AND PkgConfig_FOUND)
    pkg_check_modules(IMPLOT QUIET IMPORTED_TARGET implot)
    if(TARGET PkgConfig::IMPLOT)
        set(LAB_IMPLOT_TARGET PkgConfig::IMPLOT)
    endif()
endif()

if(NOT LAB_IMPLOT_TARGET)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/External/implot/implot.cpp")
        # ImPlot is not packaged consistently; keep only this small vendored fallback.
        add_library(lab_implot STATIC
            External/implot/implot.cpp
            External/implot/implot_items.cpp
        )
        target_include_directories(lab_implot PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/External/implot
        )
        target_link_libraries(lab_implot PUBLIC ${LAB_IMGUI_TARGET})
        set(LAB_IMPLOT_TARGET lab_implot)
    else()
        message(FATAL_ERROR
            "ImPlot development package was not found and External/implot fallback is missing. "
            "Install implot-devel or provide a CMake target implot::implot, implot, ImPlot::ImPlot, "
            "or pkg-config module implot.")
    endif()
endif()

set(SOURCES
    ui/continuity_ui.cpp
    ui/approximation.cpp
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
)

add_executable(LabApp ${SOURCES})

target_include_directories(LabApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LabApp PRIVATE
    glfw
    OpenGL::GL
    Threads::Threads
    ${LAB_IMGUI_TARGET}
    ${LAB_IMPLOT_TARGET}
)
