#include "approximation.h"

#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "../IFunction.h"
#include "../Interval.h"
#include "../PiecewiseFunction.h"
#include "piecewise_ui_utils.h"

class QuadraticApproximationFunction final : public IFunction<double, double> // S(x) = y0 + slope(x-x0) + quadratic(x-x0)^2
{
private:
    double x0_;
    double y0_;
    double slope_;
    double quadratic_;

    double Derivative(double x) const
    {
        return slope_ + 2.0 * quadratic_ * (x - x0_);
    }

public:
    QuadraticApproximationFunction(double x0, double y0, double slope, double quadratic)
        : x0_(x0), y0_(y0), slope_(slope), quadratic_(quadratic)
    {
        if (!std::isfinite(x0_) ||
            !std::isfinite(y0_) ||
            !std::isfinite(slope_) ||
            !std::isfinite(quadratic_))
        {
            throw std::invalid_argument("Quadratic approximation coefficients must be finite");
        }
    }

    double Evaluate(const double& x) const override
    {
        const double dx = x - x0_;
        const double value = y0_ + slope_ * dx + quadratic_ * dx * dx;
        if (!std::isfinite(value))
        {
            throw std::overflow_error("Quadratic approximation value is not finite");
        }

        return value;
    }

    bool IsDefinedOn(const Interval<double>&) const override
    {
        return true;
    }

    bool IsContinuousOn(const Interval<double>&) const override
    {
        return true;
    }

    Monotonicity GetMonotonicityOn(const Interval<double>& interval) const override
    {
        if (interval.GetLeft() == interval.GetRight())
        {
            return Monotonicity::Constant;
        }

        if (slope_ == 0.0 && quadratic_ == 0.0)
        {
            return Monotonicity::Constant;
        }

        const double left_derivative = Derivative(interval.GetLeft());
        const double right_derivative = Derivative(interval.GetRight());

        if (left_derivative >= 0.0 && right_derivative >= 0.0)
        {
            return Monotonicity::Increasing;
        }

        if (left_derivative <= 0.0 && right_derivative <= 0.0)
        {
            return Monotonicity::Decreasing;
        }

        return Monotonicity::NotMonotone;
    }

    UniquePtr<IFunction<double, double>> Clone() const override
    {
        return UniquePtr<IFunction<double, double>>(new QuadraticApproximationFunction(*this));
    }
};

void ApproximateFunction(UiState& state)
{
    int function_index = 0;
    if (!SelectFunctionIndex(state, "Select function to approximate: ", function_index))
    {
        return;
    }

    Interval<double> interval;
    if (!ReadInterval(interval))
    {
        return;
    }

    int point_count = 0;
    if (!ReadInt("Point count: ", point_count))
    {
        return;
    }

    try
    {
        if (point_count < 2)
        {
            std::cout << "Point count must be at least 2.\n";
            return;
        }

        const double left = interval.GetLeft();
        const double right = interval.GetRight();
        if (!(left < right))
        {
            std::cout << "Approximation interval must have positive length.\n";
            return;
        }

        const PiecewiseFunction<double, double>& source = state.functions[function_index];
        if (!source.IsContinuousOn(interval))
        {
            std::cout << "Selected function must be continuous on the approximation interval.\n";
            return;
        }

        const double step = (right - left) / static_cast<double>(point_count - 1);
        if (!(step > 0.0) || !std::isfinite(step))
        {
            std::cout << "Invalid grid step.\n";
            return;
        }

        PiecewiseFunction<double, double> approximation;

        double x0 = left;
        double y0 = source.Evaluate(x0);
        double x1 = left + step;
        double y1 = source.Evaluate(x1);
        double slope = (y1 - y0) / (x1 - x0);
        if (!std::isfinite(y0) || !std::isfinite(y1) || !std::isfinite(slope))
        {
            std::cout << "Initial approximation data is not finite.\n";
            return;
        }

        for (int i = 0; i + 1 < point_count; i++)
        {
            x1 = (i + 2 == point_count) ? right : left + step * static_cast<double>(i + 1);
            y1 = source.Evaluate(x1);

            const double h = x1 - x0;
            if (!(h > 0.0) || !std::isfinite(h) || !std::isfinite(y1))
            {
                std::cout << "Invalid approximation data.\n";
                return;
            }

            const double quadratic = (y1 - y0 - slope * h) / (h * h);
            QuadraticApproximationFunction piece_function(x0, y0, slope, quadratic);
            approximation.Define(Interval<double>(x0, x1), piece_function);

            slope += 2.0 * quadratic * h;
            if (!std::isfinite(slope))
            {
                std::cout << "Approximation derivative is not finite.\n";
                return;
            }

            x0 = x1;
            y0 = y1;
        }

        state.functions.push_back(approximation);

        std::cout << "Approximation created as function #" << state.functions.size() << ".\n";
        PrintFunction(state.functions.back());
    }
    catch (const std::exception& error)
    {
        std::cout << "Operation failed: " << error.what() << "\n";
    }
}
