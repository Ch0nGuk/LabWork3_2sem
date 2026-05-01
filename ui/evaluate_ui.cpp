#include "evaluate_ui.h"

#include <exception>
#include <iostream>

#include "piecewise_ui_utils.h"

void EvaluateInUi(UiState& state)
{
    int function_index = 0;
    if (!SelectFunctionIndex(state, "Select function: ", function_index))
    {
        return;
    }

    double x = 0.0;
    if (!ReadDouble("Write x: ", x))
    {
        return;
    }

    try
    {
        const double value = state.functions[function_index].Evaluate(x);
        std::cout << "f(" << x << ") = " << value << "\n";
    }
    catch (const std::exception& error)
    {
        std::cout << "Operation failed: " << error.what() << "\n";
    }
}
