#pragma once

#include <memory>

#include "IFunction.h"

template <typename X>
class LinearFunction : public IFunction<X, X>
{
public:
    X Evaluate(const X& x) const override
    {
        return static_cast<X>(5) * x + static_cast<X>(7);
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
        return Monotonicity::Increasing; // slope = 5 > 0
    }

    std::unique_ptr<IFunction<X, X>> Clone() const override
    {
        return std::make_unique<Linear5xPlus7Function<X>>(*this);
    }
};

