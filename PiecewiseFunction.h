#pragma once

#include <stdexcept>
#include <utility>

#include "Interval.h"
#include "IFunction.h"
#include "Piece.h"
#include "LabWork2_2sem/MutableArraySequence.h"

template <typename X, typename Y>
class PiecewiseFunction
{
private:
    MutableArraySequence<Piece<X, Y>> pieces_;

    static Monotonicity MergeMonotonicity(Monotonicity current, Monotonicity next)
    {
        if (current == Monotonicity::NotMonotone || next == Monotonicity::NotMonotone)
        {
            return Monotonicity::NotMonotone;
        }

        if (current == Monotonicity::Constant)
        {
            return next;
        }

        if (next == Monotonicity::Constant)
        {
            return current;
        }

        if (current == next)
        {
            return current;
        }

        return Monotonicity::NotMonotone;
    }

    void ValidateStructure() const
    {
        const int count = pieces_.GetLength();
        for (int i = 0; i + 1 < count; i++)
        {
            const Interval<X>& current = pieces_.Get(i).GetInterval();
            const Interval<X>& next = pieces_.Get(i + 1).GetInterval();

            if (current.GetRight() > next.GetLeft())
            {
                throw std::logic_error("PiecewiseFunction: intervals overlap");
            }
        }
    }

    int FindPieceIndexContaining(const X& x) const
    {
        const int count = pieces_.GetLength();
        for (int i = 0; i < count; i++)
        {
            const Interval<X>& interval = pieces_.Get(i).GetInterval();
            if (!interval.Contains(x))
            {
                continue;
            }

            const int right_index = i + 1;
            if (right_index < count)
            {
                const Interval<X>& right_interval = pieces_.Get(right_index).GetInterval();
                if (right_interval.Contains(x))
                {
                    return right_index;
                }
            }

            return i;
        }
        return -1;
    }

public:
    PiecewiseFunction() = default;

    PiecewiseFunction(const PiecewiseFunction& other) : pieces_(other.pieces_) {}

    PiecewiseFunction& operator=(const PiecewiseFunction& other)
    {
        pieces_ = other.pieces_;
        return *this;
    }

    PiecewiseFunction(PiecewiseFunction&&) noexcept = default;
    PiecewiseFunction& operator=(PiecewiseFunction&&) noexcept = default;

    int GetPieceCount() const
    {
        return pieces_.GetLength();
    }

    const Piece<X, Y>& GetPiece(int index) const
    {
        return pieces_.Get(index);
    }

    bool IsDefinedAt(const X& x) const
    {
        int index = FindPieceIndexContaining(x);
        if (index < 0)
        {
            return false;
        }

        const IFunction<X, Y>& function = pieces_.Get(index).GetFunction();
        return function.IsDefinedOn(Interval<X>(x, x));
    }

    Y Evaluate(const X& x) const
    {
        int index = FindPieceIndexContaining(x);
        if (index < 0)
        {
            throw std::out_of_range("PiecewiseFunction: x is outside the domain");
        }

        const IFunction<X, Y>& function = pieces_.Get(index).GetFunction();
        if (!function.IsDefinedOn(Interval<X>(x, x)))
        {
            throw std::domain_error("PiecewiseFunction: function is undefined at x");
        }

        return function.Evaluate(x);
    }

    void Define(const Interval<X>& interval, const IFunction<X, Y>& function)
    {
        MutableArraySequence<Piece<X, Y>> result;
        const int n = pieces_.GetLength();
        bool inserted = false;

        for (int i = 0; i < n; i++)
        {
            const Piece<X, Y>& piece = pieces_.Get(i);
            const Interval<X>& old_interval = piece.GetInterval();

            if (old_interval.GetRight() < interval.GetLeft())
            {
                result.Append(piece);
                continue;
            }

            if (old_interval.GetLeft() > interval.GetRight())
            {
                if (!inserted)
                {
                    result.Append(Piece<X, Y>(interval, function.Clone()));
                    inserted = true;
                }

                result.Append(piece);
                continue;
            }

            if (old_interval.GetLeft() < interval.GetLeft())
            {
                const Interval<X> left_remainder(old_interval.GetLeft(), interval.GetLeft());
                result.Append(Piece<X, Y>(left_remainder, piece.GetFunction().Clone()));
            }

            if (!inserted)
            {
                result.Append(Piece<X, Y>(interval, function.Clone()));
                inserted = true;
            }

            if (old_interval.GetRight() > interval.GetRight())
            {
                const Interval<X> right_remainder(interval.GetRight(), old_interval.GetRight());
                result.Append(Piece<X, Y>(right_remainder, piece.GetFunction().Clone()));
            }
        }

        if (!inserted)
        {
            result.Append(Piece<X, Y>(interval, function.Clone()));
        }

        pieces_ = std::move(result);
        ValidateStructure();
    }

    bool IsContinuousOn(const Interval<X>& interval) const
    {
        const int count = pieces_.GetLength();
        bool has_coverage = false;
        X covered_right = interval.GetLeft();
        int previous_piece_index = -1;

        for (int i = 0; i < count; i++)
        {
            const Interval<X>& piece_interval = pieces_.Get(i).GetInterval();
            const auto intersection = piece_interval.Intersection(interval);
            if (!intersection.has_value())
            {
                continue;
            }

            const Interval<X>& covered_part = intersection.value();
            const IFunction<X, Y>& function = pieces_.Get(i).GetFunction();
            if (!function.IsContinuousOn(covered_part))
            {
                return false;
            }

            if (!has_coverage)
            {
                if (covered_part.GetLeft() != interval.GetLeft()) // проверка на первое пересечение и дыру между левыми границами
                {
                    return false;
                }

                has_coverage = true;
                covered_right = covered_part.GetRight();
                previous_piece_index = i;
                continue;
            }

            if (covered_part.GetLeft() > covered_right)
            {
                return false;
            }

            const Piece<X, Y>& previous_piece = pieces_.Get(previous_piece_index);
            const Interval<X>& previous_interval = previous_piece.GetInterval();
            if (previous_interval.GetRight() == piece_interval.GetLeft())
            {
                const X junction = piece_interval.GetLeft(); // осхраняется точка стыка 
                const IFunction<X, Y>& previous_function = previous_piece.GetFunction(); // функция из предыдущего куска    

                if (!previous_function.IsDefinedOn(Interval<X>(junction, junction)) ||
                    !function.IsDefinedOn(Interval<X>(junction, junction))) // обе функции определены в точке (иначе разрыв)
                {
                    return false;
                }

                if (!(previous_function.Evaluate(junction) == function.Evaluate(junction))) // проверка совпадений значений
                {
                    return false;
                }
            }

            if (covered_part.GetRight() > covered_right) // смещение границы покрытия 
            {
                covered_right = covered_part.GetRight();
            }

            previous_piece_index = i;
        }

        if (!has_coverage)
        {
            return false;
        }

        return covered_right >= interval.GetRight();
    }

    Monotonicity GetMonotonicityOn(const Interval<X>& interval) const
    {
        if (interval.GetLeft() == interval.GetRight())
        {
            return IsDefinedAt(interval.GetLeft()) ? Monotonicity::Constant : Monotonicity::NotMonotone;
        }

        const int count = pieces_.GetLength();
        bool has_coverage = false;
        X covered_right = interval.GetLeft();
        int previous_piece_index = -1;
        Monotonicity overall = Monotonicity::Constant;

        for (int i = 0; i < count; i++)
        {
            const Interval<X>& piece_interval = pieces_.Get(i).GetInterval();
            const auto intersection = piece_interval.Intersection(interval);
            if (!intersection.has_value())
            {
                continue;
            }

            const Interval<X>& covered_part = intersection.value();
            const IFunction<X, Y>& function = pieces_.Get(i).GetFunction();
            const Monotonicity local = function.GetMonotonicityOn(covered_part);
            overall = MergeMonotonicity(overall, local);
            if (overall == Monotonicity::NotMonotone)
            {
                return Monotonicity::NotMonotone;
            }

            if (!has_coverage)
            {
                if (covered_part.GetLeft() != interval.GetLeft())
                {
                    return Monotonicity::NotMonotone;
                }

                has_coverage = true;
                covered_right = covered_part.GetRight();
                previous_piece_index = i;
                continue;
            }

            if (covered_part.GetLeft() > covered_right)
            {
                return Monotonicity::NotMonotone;
            }

            const Piece<X, Y>& previous_piece = pieces_.Get(previous_piece_index);
            const Interval<X>& previous_interval = previous_piece.GetInterval();
            if (previous_interval.GetRight() == piece_interval.GetLeft())
            {
                const X junction = piece_interval.GetLeft();
                const IFunction<X, Y>& previous_function = previous_piece.GetFunction();

                if (!previous_function.IsDefinedOn(Interval<X>(junction, junction)) ||
                    !function.IsDefinedOn(Interval<X>(junction, junction)))
                {
                    return Monotonicity::NotMonotone;
                }

                const Y previous_value = previous_function.Evaluate(junction);
                const Y current_value = function.Evaluate(junction);

                Monotonicity junction_trend = Monotonicity::Constant;
                if (previous_value < current_value)
                {
                    junction_trend = Monotonicity::Increasing;
                }
                else if (previous_value > current_value)
                {
                    junction_trend = Monotonicity::Decreasing;
                }

                overall = MergeMonotonicity(overall, junction_trend);
                if (overall == Monotonicity::NotMonotone)
                {
                    return Monotonicity::NotMonotone;
                }
            }

            if (covered_part.GetRight() > covered_right)
            {
                covered_right = covered_part.GetRight();
            }

            previous_piece_index = i;
        }

        if (!has_coverage || covered_right < interval.GetRight())
        {
            return Monotonicity::NotMonotone;
        }

        return overall;
    }
};
