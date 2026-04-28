#pragma once

#include <cmath>
#include <memory>

#include "IFunction.h"

template <typename X>
class ExpFunction : public IFunction<X, X>
{
public:
    X Evaluate(const X& x) const override
    {
        using std::exp;
        return exp(x);
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
        if (interval.GetLeft() == interval.GetRight())
        {
            return Monotonicity::Constant;
        }
        return Monotonicity::Increasing;
    }

    std::unique_ptr<IFunction<X, X>> Clone() const override
    {
        return std::make_unique<ExpFunction<X>>(*this);
    }
};

