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
    Piece(const Interval<X>& interval_arg, std::unique_ptr<IFunction<X, Y>> function_arg)
        : interval(interval_arg), function(std::move(function_arg))
    {
        if (!function)
        {
            throw std::invalid_argument("Piece: function is nullptr");
        }
    }

    Piece(const Piece& other)
        : interval(other.interval),
          function(other.function ? other.function->Clone() : nullptr)
    {
        if (!function)
        {
            throw std::runtime_error("Piece: failed to clone function");
        }
    }

    Piece& operator=(const Piece& other)
    {
        if (this == &other)
        {
            return *this;
        }

        interval = other.interval;
        function = other.function ? other.function->Clone() : nullptr;

        if (!function)
        {
            throw std::runtime_error("Piece: failed to clone function");
        }

        return *this;
    }

    Piece(Piece&& other) noexcept = default;
    Piece& operator=(Piece&& other) noexcept = default;

    const Interval<X>& GetInterval() const
    {
        return interval;
    }

    const IFunction<X, Y>& GetFunction() const
    {
        if (!function)
        {
            throw std::runtime_error("Piece: function is nullptr");
        }
        return *function;
    }
};
