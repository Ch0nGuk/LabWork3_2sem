#include "print_functions_ui.h"

#include <iostream>

#include "piecewise_ui_utils.h"

void PrintFunctionsInUi(const UiState& state)
{
    if (state.functions.empty())
    {
        std::cout << "No functions created.\n";
        return;
    }

    for (std::size_t index = 0; index < state.functions.size(); index++)
    {
        std::cout << "Function #" << index + 1 << ":\n";
        PrintFunction(state.functions[index]);
    }
}
