#include "define_piece_ui.h"

#include <exception>
#include <iostream>

#include "../ExpFunction.h"
#include "../InverseSquareFunction.h"
#include "../LinearFunction.h"
#include "../PolynomialFunction.h"
#include "../SinFunction.h"
#include "piecewise_ui_utils.h"

namespace
{
    void PrintBaseFunctionMenu()
    {
        std::cout << "Select function type:\n";
        std::cout << "1. 5x + 7\n";
        std::cout << "2. exp(x)\n";
        std::cout << "3. sin(x)\n";
        std::cout << "4. x^4 - 3x^3 + 7x\n";
        std::cout << "5. 1 / x^2\n";
    }

    bool DefineByChoice(PiecewiseFunction<double, double>& piecewise_function,
                        const Interval<double>& interval,
                        int choice)
    {
        switch (choice)
        {
        case 1:
        {
            LinearFunction<double> function;
            piecewise_function.Define(interval, function);
            return true;
        }
        case 2:
        {
            ExpFunction<double> function;
            piecewise_function.Define(interval, function);
            return true;
        }
        case 3:
        {
            SinFunction<double> function;
            piecewise_function.Define(interval, function);
            return true;
        }
        case 4:
        {
            PolynomialFunction<double> function;
            piecewise_function.Define(interval, function);
            return true;
        }
        case 5:
        {
            InverseSquareFunction<double> function;
            piecewise_function.Define(interval, function);
            return true;
        }
        default:
            std::cout << "Unknown function type.\n";
            return false;
        }
    }
}

void DefinePieceInUi(UiState& state)
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

    PrintBaseFunctionMenu();

    int function_type = 0;
    if (!ReadInt("Choice: ", function_type))
    {
        return;
    }

    try
    {
        if (!DefineByChoice(state.functions[function_index], interval, function_type))
        {
            return;
        }

        std::cout << "Function updated.\n";
        PrintFunction(state.functions[function_index]);
    }
    catch (const std::exception& error)
    {
        std::cout << "Operation failed: " << error.what() << "\n";
    }
}
