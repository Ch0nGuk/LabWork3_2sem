#include "create_function_ui.h"

#include <exception>
#include <iostream>

void CreateFunctionInUi(UiState& state)
{
    try
    {
        state.functions.emplace_back();
        std::cout << "Created function #" << state.functions.size() << ".\n";
    }
    catch (const std::exception& error)
    {
        std::cout << "Operation failed: " << error.what() << "\n";
    }
}
