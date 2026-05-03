#include <iostream>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <limits>

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
    MutableArraySequence<MutableArraySequence<double>> x_coords;
    MutableArraySequence<MutableArraySequence<double>> y_coords;
};


PlotData GeneratePlotPoints(const PiecewiseFunction<double, double>& func, int points_count = 1000) {
    PlotData data;

    for (int j = 0; j < func.GetPieceCount(); j++)
    {
        Piece piece = func.GetPiece(j);

        double x_max = piece.GetInterval().GetRight();
        double x_min = piece.GetInterval().GetLeft();
        double step = (x_max - x_min) / (points_count - 1);

        MutableArraySequence<double> x_inner;
        MutableArraySequence<double> y_inner;

        for (int i = 0; i < points_count; ++i) {
            double x = x_min + i * step;
            try 
            {
                double y = piece.GetFunction().Evaluate(x);
                if (std::abs(y) > 1e6) 
                {
                    x_inner.Append(x);
                    y_inner.Append(std::numeric_limits<double>::quiet_NaN());
                }
                else
                {
                    x_inner.Append(x);
                    y_inner.Append(y);
                }
            } 
            catch (const std::out_of_range&) {
                continue; 
            }
        }

        data.x_coords.Append(x_inner);
        data.y_coords.Append(y_inner);
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
            
            for (int i = 0; i < selected_func.GetPieceCount(); i++)
            {
                std::string label = "Piece " + std::to_string(i);
                ImPlot::PlotLine(label.c_str(), data.x_coords[i].GetData(), data.y_coords[i].GetData(), (int)data.x_coords[i].GetLength());
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