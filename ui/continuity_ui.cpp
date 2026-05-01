#include "continuity_ui.h"

#include <exception>
#include <iostream>

#include "piecewise_ui_utils.h"

void CheckContinuityInUi(UiState& state)
{
    int function_index = 0;
    if (!SelectFunctionIndex(state, "Select function: ", function_index))
    {
        return;
    }

    Interval<double> interval;
    if (!ReadInterval(interval))
    {
        return;
    }

    try
    {
        const bool is_continuous = state.functions[function_index].IsContinuousOn(interval);
        std::cout << "Continuity: " << (is_continuous ? "continuous" : "not continuous") << "\n";
    }
    catch (const std::exception& error)
    {
        std::cout << "Operation failed: " << error.what() << "\n";
    }
}
