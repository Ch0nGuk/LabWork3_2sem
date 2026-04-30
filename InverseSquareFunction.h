#pragma once

#include <stdexcept>
#include "IFunction.h"

template <typename X>
class InverseSquareFunction : public IFunction<X, X>
{
public:
    X Evaluate(const X& x) const override
    {
        if (x == static_cast<X>(0))
        {
            throw std::domain_error("inverse square is undefined at zero");
        }

        return static_cast<X>(1) / (x * x);
    }

    bool IsDefinedOn(const Interval<X>& interval) const override
    {
        return !interval.Contains(static_cast<X>(0));
    }

    bool IsContinuousOn(const Interval<X>& interval) const override
    {
        return IsDefinedOn(interval);
    }

    Monotonicity GetMonotonicityOn(const Interval<X>& interval) const override
    {
        if (!IsDefinedOn(interval))
        {
            return Monotonicity::NotMonotone;
        }

        if (interval.GetLeft() == interval.GetRight())
        {
            return Monotonicity::Constant;
        }

        if (interval.GetRight() < static_cast<X>(0))
        {
            return Monotonicity::Increasing;
        }

        if (interval.GetLeft() > static_cast<X>(0))
        {
            return Monotonicity::Decreasing;
        }

        return Monotonicity::NotMonotone;
    }

    UniquePtr<IFunction<X, X>> Clone() const override
    {
        return UniquePtr<IFunction<X, X>>(new InverseSquareFunction<X>(*this));
    }
};
