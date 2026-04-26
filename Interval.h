#pragma once
#include <stdexcept>
#include <optional>
#include <algorithm>

template <typename X>
class Interval
{
private:
    X left;
    X right;

public:
    Interval(const X& left_arg, const X& right_arg) : left(left_arg), right(right_arg)
    {
        if (right_arg < left_arg)
        {
            throw std::invalid_argument("right < left");
        }
    }

    Interval(const Interval& other) : left(other.left), right(other.right) {}

    const X& GetLeft() const
    {
        return left;
    }

    const X& GetRight() const
    {
        return right;
    }

    bool Contains(const X& x) const
    {
        return (x >= left && x <= right);
    }
    
    bool operator==(const Interval<X>& other) const
    {
        return left == other.left && right == other.right;
    }

    std::optional<Interval<X>> Intersection(const Interval<X>& other) const
    {
        if (other.right < left || other.left > right) return std::nullopt;

        X new_left = std::max(left, other.left);
        X new_right = std::min(right, other.right);

        return Interval<X>(new_left, new_right);
    }
};
