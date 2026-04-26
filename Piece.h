#pragma once

#include <memory>
#include <stdexcept>
#include "Interval.h"
#include "IFunction.h"

template <typename X, typename Y>
class Piece
{
private:
    Interval<X> interval;
    std::unique_ptr<IFunction<X, Y>> function;

public:
    Piece(const Interval<X>& interval_arg, std::unique_ptr<IFunction<X, Y>> function_arg);

    Piece(const Piece& other);
    Piece& operator=(const Piece& other);

    Piece(Piece&& other) noexcept = default;
    Piece& operator=(Piece&& other) noexcept = default;

    const Interval<X>& GetInterval() const;
    const IFunction<X, Y>& GetFunction() const;
};
