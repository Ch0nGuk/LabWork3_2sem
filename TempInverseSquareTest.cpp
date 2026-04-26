#include <iostream>
#include <memory>
#include <stdexcept>

#include "InverseSquareFunction.h"

const char* ToString(Monotonicity monotonicity)
{
    switch (monotonicity)
    {
    case Monotonicity::Increasing:
        return "Increasing";
    case Monotonicity::Decreasing:
        return "Decreasing";
    case Monotonicity::Constant:
        return "Constant";
    case Monotonicity::NotMonotone:
        return "NotMonotone";
    default:
        return "Unknown";
    }
}

void PrintIntervalReport(const IFunction<double, double>& function, const Interval<double>& interval)
{
    std::cout << "[" << interval.GetLeft() << ", " << interval.GetRight() << "]\n";
    std::cout << "  defined: " << function.IsDefinedOn(interval) << '\n';
    std::cout << "  continuous: " << function.IsContinuousOn(interval) << '\n';
    std::cout << "  monotonicity: " << ToString(function.GetMonotonicityOn(interval)) << '\n';
}

int main()
{
    InverseSquareFunction<double> function;

    std::cout << "Evaluate(2): " << function.Evaluate(2.0) << '\n';

    try
    {
        std::cout << "Evaluate(0): " << function.Evaluate(0.0) << '\n';
    }
    catch (const std::exception& exception)
    {
        std::cout << "Evaluate(0) threw: " << exception.what() << '\n';
    }

    PrintIntervalReport(function, Interval<double>(-4.0, -1.0));
    PrintIntervalReport(function, Interval<double>(1.0, 4.0));
    PrintIntervalReport(function, Interval<double>(-2.0, 2.0));
    PrintIntervalReport(function, Interval<double>(3.0, 3.0));

    std::unique_ptr<IFunction<double, double>> clone = function.Clone();
    std::cout << "Clone Evaluate(4): " << clone->Evaluate(4.0) << '\n';

    return 0;
}
