#include "piecewise_ui_utils.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "../ExpFunction.h"
#include "../InverseSquareFunction.h"
#include "../LinearFunction.h"
#include "../PolynomialFunction.h"
#include "../SinFunction.h"

void ConfigureFloatingPointOutput()
{
    std::cout << std::setprecision(15);
}

bool ReadInt(const std::string& prompt, int& value)
{
    std::cout << prompt;

    std::string line;
    if (!std::getline(std::cin, line))
    {
        if (std::cin.eof())
        {
            return false;
        }

        std::cin.clear();
        std::cout << "Invalid input.\n";
        return false;
    }

    std::istringstream stream(line);
    std::string extra;
    if (!(stream >> value) || (stream >> extra))
    {
        std::cout << "Invalid input.\n";
        return false;
    }

    return true;
}

bool ReadDouble(const std::string& prompt, double& value)
{
    std::cout << prompt;

    std::string line;
    if (!std::getline(std::cin, line))
    {
        if (std::cin.eof())
        {
            return false;
        }

        std::cin.clear();
        std::cout << "Invalid input.\n";
        return false;
    }

    std::istringstream stream(line);
    std::string extra;
    if (!(stream >> value) || (stream >> extra))
    {
        std::cout << "Invalid input.\n";
        return false;
    }

    if (!std::isfinite(value))
    {
        std::cout << "Invalid input: value must be finite.\n";
        return false;
    }

    return true;
}

bool ReadInterval(Interval<double>& interval)
{
    double left = 0.0;
    double right = 0.0;

    if (!ReadDouble("Write left border: ", left) ||
        !ReadDouble("Write right border: ", right))
    {
        return false;
    }

    try
    {
        interval = Interval<double>(left, right);
    }
    catch (const std::exception& error)
    {
        std::cout << "Invalid interval: " << error.what() << "\n";
        return false;
    }

    return true;
}

void PrintStoredFunctions(const UiState& state)
{
    if (state.functions.empty())
    {
        std::cout << "No functions created.\n";
        return;
    }

    std::cout << "Stored piecewise functions:\n";
    for (std::size_t index = 0; index < state.functions.size(); index++)
    {
        std::cout << index + 1 << ". "
                  << state.functions[index].GetPieceCount()
                  << " piece(s)\n";
    }
}

void PrintFunction(const PiecewiseFunction<double, double>& function)
{
    const int piece_count = function.GetPieceCount();
    if (piece_count == 0)
    {
        std::cout << "  empty function\n";
        return;
    }

    for (int index = 0; index < piece_count; index++)
    {
        const Piece<double, double>& piece = function.GetPiece(index);
        const Interval<double>& interval = piece.GetInterval();

        std::cout << "  " << index + 1 << ". ["
                  << interval.GetLeft() << ", "
                  << interval.GetRight() << "] -> "
                  << FunctionName(piece.GetFunction()) << "\n";
    }
}

bool SelectFunctionIndex(const UiState& state, const std::string& prompt, int& index)
{
    if (state.functions.empty())
    {
        std::cout << "No functions available.\n";
        return false;
    }

    PrintStoredFunctions(state);

    int choice = 0;
    if (!ReadInt(prompt, choice))
    {
        return false;
    }

    if (choice <= 0 || choice > static_cast<int>(state.functions.size()))
    {
        std::cout << "Wrong number.\n";
        return false;
    }

    index = choice - 1;
    return true;
}

const char* FunctionName(const IFunction<double, double>& function)
{
    if (dynamic_cast<const LinearFunction<double>*>(&function) != nullptr)
    {
        return "5x + 7";
    }

    if (dynamic_cast<const ExpFunction<double>*>(&function) != nullptr)
    {
        return "exp(x)";
    }

    if (dynamic_cast<const SinFunction<double>*>(&function) != nullptr)
    {
        return "sin(x)";
    }

    if (dynamic_cast<const PolynomialFunction<double>*>(&function) != nullptr)
    {
        return "x^4 - 3x^3 + 7x";
    }

    if (dynamic_cast<const InverseSquareFunction<double>*>(&function) != nullptr)
    {
        return "1 / x^2";
    }

    return "unknown function";
}

const char* MonotonicityToString(Monotonicity monotonicity)
{
    switch (monotonicity)
    {
    case Monotonicity::Increasing:
        return "increasing";
    case Monotonicity::Decreasing:
        return "decreasing";
    case Monotonicity::Constant:
        return "constant";
    case Monotonicity::NotMonotone:
        return "not monotone";
    default:
        return "unknown";
    }
}
