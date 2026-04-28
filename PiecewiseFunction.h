#pragma once

#include <memory>
#include <stdexcept>

#include "Interval.h"
#include "IFunction.h"
#include "Piece.h"
#include "LabWork2_2sem/MutableArraySequence.h"

template <typename X, typename Y>
class PiecewiseFunction
{
private:
    MutableArraySequence<Piece<X, Y>> pieces_;

    int FindPieceIndexContaining(const X& x) const
    {
        for (int i = pieces_.GetLength() - 1; i >= 0; i--)
        {
            if (pieces_.Get(i).GetInterval().Contains(x))
            {
                return i;
            }
        }
        return -1;
    }

public:
    PiecewiseFunction() = default;

    PiecewiseFunction(const PiecewiseFunction& other) : pieces_(other.pieces_) {}

    PiecewiseFunction& operator=(const PiecewiseFunction& other)
    {
        pieces_ = other.pieces_;
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
        MutableArraySequence<Piece<X, Y>> new_pieces;
        int n = pieces_.GetLength();
        for (int i = 0; i < n; i++)
        {
            const Piece<X, Y>& piece = pieces_.Get(i);
            const Interval<X>& old_i = piece.GetInterval();

            auto intersection = old_i.Intersection(interval);
            if (!intersection.has_value())
            {
                new_pieces.Append(piece);
                continue;
            }

            if (old_i.GetLeft() < interval.GetLeft())
            {
                Interval<X> left_rem(old_i.GetLeft(), interval.GetLeft());
                new_pieces.Append(Piece<X, Y>(left_rem, piece.GetFunction().Clone()));
            }

            if (old_i.GetRight() > interval.GetRight())
            {
                Interval<X> right_rem(interval.GetRight(), old_i.GetRight());
                new_pieces.Append(Piece<X, Y>(right_rem, piece.GetFunction().Clone()));
            }
        }

        Piece<X, Y> new_piece(interval, function.Clone());

        const int m = new_pieces.GetLength();
        int insert_pos = m;
        for (int i = 0; i < m; i++)
        {
            if (interval.GetLeft() < new_pieces.Get(i).GetInterval().GetLeft())
            {
                insert_pos = i;
                break;
            }
        }

        MutableArraySequence<Piece<X, Y>> final_pieces;
        for (int i = 0; i < insert_pos; i++)
        {
            final_pieces.Append(new_pieces.Get(i));
        }
        final_pieces.Append(new_piece);
        for (int i = insert_pos; i < m; i++)
        {
            final_pieces.Append(new_pieces.Get(i));
        }

        pieces_ = std::move(final_pieces);
    }

    bool IsContinuousOn(const Interval<X>& interval) const
    {
        int n = pieces_.GetLength();
        bool has_any = false;
        int prev_index = -1;

        for (int i = 0; i < n; i++)
        {
            const Interval<X>& piece_interval = pieces_.Get(i).GetInterval();
            auto inter = piece_interval.Intersection(interval);
            if (!inter.has_value())
            {
                continue;
            }

            has_any = true;
            const IFunction<X, Y>& f = pieces_.Get(i).GetFunction();
            if (!f.IsContinuousOn(inter.value()))
            {
                return false;
            }

            if (prev_index >= 0)
            {
                // Check continuity at the junction point if intervals touch.
                const Interval<X>& prev_int = pieces_.Get(prev_index).GetInterval();
                const Interval<X>& cur_int = piece_interval;

                // Touching at x0 if prev.right == cur.left.
                if (prev_int.GetRight() == cur_int.GetLeft())
                {
                    const X x0 = cur_int.GetLeft();
                    const IFunction<X, Y>& f_left = pieces_.Get(prev_index).GetFunction();
                    const IFunction<X, Y>& f_right = f;

                    const bool left_defined = f_left.IsDefinedOn(Interval<X>(x0, x0));
                    const bool right_defined = f_right.IsDefinedOn(Interval<X>(x0, x0));
                    if (left_defined && right_defined)
                    {
                        if (!(f_left.Evaluate(x0) == f_right.Evaluate(x0)))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        // If one side isn't defined at the border point, treat as discontinuity on a closed interval.
                        return false;
                    }
                }
            }

            prev_index = i;
        }

        if (!has_any)
        {
            // No pieces intersect interval => not continuous "as a function on that interval"
            return false;
        }

        return true;
    }

    Monotonicity GetMonotonicityOn(const Interval<X>& interval) const
    {
        const int n = pieces_.GetLength();
        bool has_any = false;

        Monotonicity overall = Monotonicity::Constant;

        for (int i = 0; i < n; i++)
        {
            const Interval<X>& piece_interval = pieces_.Get(i).GetInterval();
            auto inter = piece_interval.Intersection(interval);
            if (!inter.has_value())
            {
                continue;
            }

            has_any = true;
            const IFunction<X, Y>& f = pieces_.Get(i).GetFunction();
            const Monotonicity m = f.GetMonotonicityOn(inter.value());
            if (m == Monotonicity::NotMonotone)
            {
                return Monotonicity::NotMonotone;
            }

            if (overall == Monotonicity::Constant)
            {
                overall = m;
            }
            else if (overall != m)
            {
                return Monotonicity::NotMonotone;
            }
        }

        if (!has_any)
        {
            return Monotonicity::NotMonotone;
        }

        return overall;
    }
};
