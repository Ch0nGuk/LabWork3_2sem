#include "monotonicity_ui.h"

#include <exception>
#include <iostream>

#include "piecewise_ui_utils.h"

void CheckMonotonicityInUi(UiState& state)
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
        const Monotonicity monotonicity = state.functions[function_index].GetMonotonicityOn(interval);
        std::cout << "Monotonicity: " << MonotonicityToString(monotonicity) << "\n";
    }
    catch (const std::exception& error)
    {
        std::cout << "Operation failed: " << error.what() << "\n";
    }
}
