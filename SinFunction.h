#pragma once

#include <cmath>
#include <memory>

#include "IFunction.h"

template <typename X>
class SinFunction : public IFunction<X, X>
{
private:
    static X Pi()
    {
        return std::acos(static_cast<X>(-1));
    }

    static bool HasCriticalPointInsideOpenInterval(const X& l, const X& r)
    {
        const X pi = Pi();
        const X start = (l - pi / static_cast<X>(2)) / pi;
        const X end = (r - pi / static_cast<X>(2)) / pi;

        // Any integer k strictly between start and end implies a critical point in (l, r).
        const long long k_min = static_cast<long long>(std::floor(start)) + 1;
        const long long k_max = static_cast<long long>(std::ceil(end)) - 1;
        return k_min <= k_max;
    }

public:
    X Evaluate(const X& x) const override
    {
        using std::sin;
        return sin(x);
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

        if (HasCriticalPointInsideOpenInterval(l, r))
        {
            return Monotonicity::NotMonotone;
        }

        // No critical points inside -> cos keeps sign on (l, r).
        // Determine direction by cos at midpoint.
        const X mid = (l + r) / static_cast<X>(2);
        using std::cos;
        const X c = cos(mid);
        if (c > static_cast<X>(0))
        {
            return Monotonicity::Increasing;
        }
        if (c < static_cast<X>(0))
        {
            return Monotonicity::Decreasing;
        }
        // Extremely rare: midpoint hits cos(mid)=0 exactly -> interval near critical, be conservative.
        return Monotonicity::NotMonotone;
    }

    std::unique_ptr<IFunction<X, X>> Clone() const override
    {
        return std::make_unique<SinFunction<X>>(*this);
    }
};

