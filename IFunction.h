#pragma once

#include "Monotonicity.h"
#include "Interval.h"
#include "UniquePtr.h"

template <typename X, typename Y> 
class IFunction
{
public:
    virtual ~IFunction() = default;

    virtual Y Evaluate(const X& x) const = 0;
    virtual bool IsDefinedOn(const Interval<X>& interval) const = 0;
    virtual bool IsContinuousOn(const Interval<X>& interval) const = 0;
    virtual Monotonicity GetMonotonicityOn(const Interval<X>& interval) const = 0;
    virtual UniquePtr<IFunction<X, Y>> Clone() const = 0;
};
