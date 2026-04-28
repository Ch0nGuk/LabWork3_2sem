#pragma once

#include <memory>

#include "IFunction.h"

// f(x) = x^4 - 3x^3 + 7x
template <typename X>
class PolynomialFunction : public IFunction<X, X>
{
private:
    static X CriticalPoint()
    {
        return static_cast<X>(-0.7622127405074577);
    }

public:
    X Evaluate(const X& x) const override
    {
        return x * x * x * x - static_cast<X>(3) * x * x * x + static_cast<X>(7) * x;
    }

    bool IsDefinedOn(const Interval<X>&) const override
    {
        return true;
    }

    bool IsContinuousOn(const Interval<X>&) const override
    {
        return true;
    }

    Monotonicity GetMonotonicityOn(const Interval<X>& interval) const override
    {
        const X l = interval.GetLeft();
        const X r = interval.GetRight();
        if (l == r)
        {
            return Monotonicity::Constant;
        }

        const X c = CriticalPoint();
        if (r <= c)
        {
            return Monotonicity::Decreasing;
        }
        if (l >= c)
        {
            return Monotonicity::Increasing;
        }
        return Monotonicity::NotMonotone;
    }

    std::unique_ptr<IFunction<X, X>> Clone() const override
    {
        return std::make_unique<Polynomial4Function<X>>(*this);
    }
};

