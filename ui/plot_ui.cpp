#include <iostream>
#include <GLFW/glfw3.h>
#include <cmath>
#include <exception>
#include <limits>
#include <string>
#include <vector>

#include "plot_ui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "piecewise_ui_utils.h"
#include "PiecewiseFunction.h"
#include "Piece.h"
#include "Interval.h"


struct PlotData {
    std::vector<std::vector<double>> x_coords;
    std::vector<std::vector<double>> y_coords;
};


PlotData GeneratePlotPoints(const PiecewiseFunction<double, double>& func, int points_count = 1000) {
    PlotData data;
    if (points_count < 2)
    {
        points_count = 2;
    }

    const int piece_count = func.GetPieceCount();
    data.x_coords.reserve(piece_count);
    data.y_coords.reserve(piece_count);

    for (int j = 0; j < piece_count; j++)
    {
        const Piece<double, double>& piece = func.GetPiece(j);

        const double x_max = piece.GetInterval().GetRight();
        const double x_min = piece.GetInterval().GetLeft();
        const double step = (x_max - x_min) / (points_count - 1);

        std::vector<double> x_inner;
        std::vector<double> y_inner;
        x_inner.reserve(points_count);
        y_inner.reserve(points_count);

        for (int i = 0; i < points_count; ++i) {
            const double x = x_min + i * step;
            x_inner.push_back(x);

            try 
            {
                const double y = piece.GetFunction().Evaluate(x);
                if (!std::isfinite(y) || std::abs(y) > 1e6) 
                {
                    y_inner.push_back(std::numeric_limits<double>::quiet_NaN());
                }
                else
                {
                    y_inner.push_back(y);
                }
            } 
            catch (const std::exception&) {
                y_inner.push_back(std::numeric_limits<double>::quiet_NaN());
            }
        }

        data.x_coords.push_back(std::move(x_inner));
        data.y_coords.push_back(std::move(y_inner));
    }
    return data;
}


void PlotFunctionInUi(UiState& state) {
    if (state.functions.empty()) {
        std::cout << "No functions available to plot.\n";
        return;
    }

    int index = 0;
    if (!ReadInt("Enter function index to plot: ", index) || index <= 0 || index > state.functions.size()) {
        std::cout << "Invalid index.\n";
        return;
    }

    auto& selected_func = state.functions[index - 1];
    
    PlotData data = GeneratePlotPoints(selected_func);

    if (!glfwInit()) return;
    GLFWwindow* window = glfwCreateWindow(1024, 1024, "Function Plot", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Plot Window", nullptr);
        ImGui::Text("Plotting function at index: %d", index);

        if (ImPlot::BeginPlot("Piecewise Plot", ImVec2(1024, 1024), ImPlotFlags_Equal)) 
        { 
            ImPlot::SetupAxes("X", "Y");

            ImPlot::SetupAxisLimits(ImAxis_X1, -5.0, 5.0, ImPlotCond_Once);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -5.0, 5.0, ImPlotCond_Once);
            
            for (std::size_t i = 0; i < data.x_coords.size(); i++)
            {
                if (data.x_coords[i].empty())
                {
                    continue;
                }

                std::string label = "Piece " + std::to_string(i + 1);
                ImPlot::PlotLine(
                    label.c_str(),
                    data.x_coords[i].data(),
                    data.y_coords[i].data(),
                    static_cast<int>(data.x_coords[i].size()));
            }
            
            ImPlot::EndPlot();
        }

        if (ImGui::Button("Close Plot")) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
        ImGui::End();

        ImGui::Render();
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    
    std::cout << "Returning to console menu...\n";
}
