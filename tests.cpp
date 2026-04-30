#include "tests.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "ExpFunction.h"
#include "Interval.h"
#include "InverseSquareFunction.h"
#include "LinearFunction.h"
#include "Piece.h"
#include "PiecewiseFunction.h"
#include "PolynomialFunction.h"
#include "SinFunction.h"

namespace
{
    template <typename T>
    void AssertEqual(const T& actual, const T& expected, const std::string& message)
    {
        if (!(actual == expected))
        {
            std::ostringstream error_stream;
            error_stream << message;
            throw std::runtime_error(error_stream.str());
        }
    }

    void AssertTrue(bool condition, const std::string& message)
    {
        if (!condition)
        {
            throw std::runtime_error(message);
        }
    }

    template <typename Exception, typename Action>
    void AssertThrowsExactly(Action action, const std::string& message)
    {
        static_assert(std::is_base_of<std::exception, Exception>::value, "Exception must derive from std::exception");

        try
        {
            action();
        }
        catch (const Exception&)
        {
            return;
        }
        catch (const std::exception&)
        {
            throw std::runtime_error(message);
        }

        throw std::runtime_error(message);
    }

    void AssertNear(double actual, double expected, double eps, const std::string& message)
    {
        if (std::fabs(actual - expected) > eps)
        {
            std::ostringstream error_stream;
            error_stream << message;
            throw std::runtime_error(error_stream.str());
        }
    }

    void TestInterval()
    {
        Interval<double> interval(1.0, 3.0);
        AssertTrue(interval.Contains(1.0), "Interval must contain left boundary");
        AssertTrue(interval.Contains(2.0), "Interval must contain inner point");
        AssertTrue(interval.Contains(3.0), "Interval must contain right boundary");
        AssertTrue(!interval.Contains(3.1), "Interval must not contain outside point");

        const auto intersection = interval.Intersection(Interval<double>(2.0, 4.0));
        AssertTrue(intersection.has_value(), "Interval intersection must exist");
        AssertEqual(intersection->GetLeft(), 2.0, "Interval intersection left boundary");
        AssertEqual(intersection->GetRight(), 3.0, "Interval intersection right boundary");

        const auto touching_intersection = interval.Intersection(Interval<double>(3.0, 5.0));
        AssertTrue(touching_intersection.has_value(), "Touching intervals must intersect at one point");
        AssertEqual(touching_intersection->GetLeft(), 3.0, "Point intersection left boundary");
        AssertEqual(touching_intersection->GetRight(), 3.0, "Point intersection right boundary");

        const auto empty_intersection = interval.Intersection(Interval<double>(4.0, 5.0));
        AssertTrue(!empty_intersection.has_value(), "Interval disjoint intersection must be empty");

        Interval<double> degenerate(2.0, 2.0);
        AssertTrue(degenerate.Contains(2.0), "Degenerate interval must contain its only point");
        AssertTrue(!degenerate.Contains(2.0001), "Degenerate interval must reject other points");

        AssertThrowsExactly<std::invalid_argument>(
            []() { Interval<double> invalid(5.0, 4.0); },
            "Interval with right < left must throw invalid_argument");
    }

    void TestBaseFunctions()
    {
        LinearFunction<double> linear;
        AssertEqual(linear.Evaluate(2.0), 17.0, "LinearFunction Evaluate");
        AssertTrue(linear.IsDefinedOn(Interval<double>(-10.0, 10.0)), "LinearFunction must be defined everywhere");
        AssertTrue(linear.IsContinuousOn(Interval<double>(-10.0, 10.0)), "LinearFunction must be continuous everywhere");
        AssertEqual(linear.GetMonotonicityOn(Interval<double>(0.0, 2.0)), Monotonicity::Increasing, "LinearFunction monotonicity");
        AssertEqual(linear.GetMonotonicityOn(Interval<double>(2.0, 2.0)), Monotonicity::Constant, "LinearFunction point monotonicity");

        ExpFunction<double> exp_function;
        AssertNear(exp_function.Evaluate(0.0), 1.0, 1e-9, "ExpFunction Evaluate at zero");
        AssertNear(exp_function.Evaluate(1.0), std::exp(1.0), 1e-9, "ExpFunction Evaluate at one");
        AssertTrue(exp_function.IsContinuousOn(Interval<double>(-3.0, 3.0)), "ExpFunction must be continuous");
        AssertEqual(exp_function.GetMonotonicityOn(Interval<double>(0.0, 2.0)), Monotonicity::Increasing, "ExpFunction monotonicity");
        AssertEqual(exp_function.GetMonotonicityOn(Interval<double>(1.0, 1.0)), Monotonicity::Constant, "ExpFunction point monotonicity");

        SinFunction<double> sin_function;
        AssertNear(sin_function.Evaluate(0.0), 0.0, 1e-9, "SinFunction Evaluate at zero");
        AssertNear(sin_function.Evaluate(std::acos(-1.0) / 2.0), 1.0, 1e-9, "SinFunction Evaluate at pi/2");
        AssertEqual(sin_function.GetMonotonicityOn(Interval<double>(-1.0, 1.0)), Monotonicity::Increasing, "SinFunction monotonicity on increasing interval");
        AssertEqual(sin_function.GetMonotonicityOn(Interval<double>(0.0, 4.0)), Monotonicity::NotMonotone, "SinFunction monotonicity on wide interval");
        AssertEqual(sin_function.GetMonotonicityOn(Interval<double>(0.5, 0.5)), Monotonicity::Constant, "SinFunction point monotonicity");

        PolynomialFunction<double> polynomial;
        AssertNear(polynomial.Evaluate(2.0), 6.0, 1e-9, "PolynomialFunction Evaluate");
        AssertEqual(polynomial.GetMonotonicityOn(Interval<double>(-2.0, -1.0)), Monotonicity::Decreasing, "PolynomialFunction left monotonicity");
        AssertEqual(polynomial.GetMonotonicityOn(Interval<double>(0.0, 2.0)), Monotonicity::Increasing, "PolynomialFunction right monotonicity");
        AssertEqual(polynomial.GetMonotonicityOn(Interval<double>(-1.0, 0.0)), Monotonicity::NotMonotone, "PolynomialFunction must be non-monotone across critical point");
        AssertEqual(polynomial.GetMonotonicityOn(Interval<double>(0.0, 0.0)), Monotonicity::Constant, "PolynomialFunction point monotonicity");

        InverseSquareFunction<double> inverse_square;
        AssertNear(inverse_square.Evaluate(2.0), 0.25, 1e-9, "InverseSquareFunction Evaluate");
        AssertTrue(inverse_square.IsDefinedOn(Interval<double>(1.0, 4.0)), "InverseSquareFunction positive interval defined");
        AssertTrue(inverse_square.IsDefinedOn(Interval<double>(-4.0, -1.0)), "InverseSquareFunction negative interval defined");
        AssertTrue(!inverse_square.IsDefinedOn(Interval<double>(-1.0, 1.0)), "InverseSquareFunction interval through zero undefined");
        AssertEqual(inverse_square.GetMonotonicityOn(Interval<double>(1.0, 4.0)), Monotonicity::Decreasing, "InverseSquareFunction positive monotonicity");
        AssertEqual(inverse_square.GetMonotonicityOn(Interval<double>(-4.0, -1.0)), Monotonicity::Increasing, "InverseSquareFunction negative monotonicity");
        AssertEqual(inverse_square.GetMonotonicityOn(Interval<double>(2.0, 2.0)), Monotonicity::Constant, "InverseSquareFunction point monotonicity");
        AssertThrowsExactly<std::domain_error>(
            [&inverse_square]() { inverse_square.Evaluate(0.0); },
            "InverseSquareFunction Evaluate at zero must throw domain_error");
    }

    void TestPiece()
    {
        LinearFunction<double> linear;
        Piece<double, double> piece(Interval<double>(0.0, 2.0), linear.Clone());
        AssertEqual(piece.GetInterval().GetLeft(), 0.0, "Piece interval left boundary");
        AssertEqual(piece.GetInterval().GetRight(), 2.0, "Piece interval right boundary");
        AssertNear(piece.GetFunction().Evaluate(1.0), 12.0, 1e-9, "Piece stored function");

        Piece<double, double> copied(piece);
        AssertNear(copied.GetFunction().Evaluate(1.0), 12.0, 1e-9, "Piece copy must preserve function");

        AssertThrowsExactly<std::invalid_argument>(
            []() { Piece<double, double> invalid(Interval<double>(0.0, 1.0), UniquePtr<IFunction<double, double>>()); },
            "Piece with nullptr function must throw invalid_argument");
    }

    void TestPiecewiseFunctionEmptyState()
    {
        PiecewiseFunction<double, double> function;
        AssertTrue(!function.IsDefinedAt(0.0), "Empty PiecewiseFunction must be undefined everywhere");
        AssertThrowsExactly<std::out_of_range>(
            [&function]() { function.Evaluate(0.0); },
            "Empty PiecewiseFunction Evaluate must throw out_of_range");
        AssertTrue(!function.IsContinuousOn(Interval<double>(0.0, 1.0)), "Empty PiecewiseFunction must not be continuous on non-empty interval");
        AssertEqual(
            function.GetMonotonicityOn(Interval<double>(0.0, 1.0)),
            Monotonicity::NotMonotone,
            "Empty PiecewiseFunction monotonicity must be NotMonotone");
    }

    void TestPiecewiseFunctionIsDefinedAt()
    {
        PiecewiseFunction<double, double> function;
        LinearFunction<double> linear;
        InverseSquareFunction<double> inverse_square;

        function.Define(Interval<double>(-2.0, 0.0), linear);
        function.Define(Interval<double>(0.0, 2.0), inverse_square);

        AssertTrue(function.IsDefinedAt(-1.0), "PiecewiseFunction must be defined inside left piece");
        AssertTrue(!function.IsDefinedAt(-3.0), "PiecewiseFunction must be undefined outside domain");
        AssertTrue(!function.IsDefinedAt(0.0), "PiecewiseFunction must be undefined at junction if chosen piece is undefined there");
        AssertTrue(function.IsDefinedAt(1.0), "PiecewiseFunction must be defined inside right piece away from singularity");

        AssertThrowsExactly<std::domain_error>(
            [&function]() { function.Evaluate(0.0); },
            "PiecewiseFunction Evaluate at undefined covered point must throw domain_error");
    }

    void TestPiecewiseFunctionDefineInsideSinglePiece()
    {
        PiecewiseFunction<double, double> function;
        LinearFunction<double> linear;
        ExpFunction<double> exp_function;

        function.Define(Interval<double>(0.0, 10.0), linear);
        AssertEqual(function.GetPieceCount(), 1, "PiecewiseFunction first Define must create one piece");
        AssertNear(function.Evaluate(5.0), 32.0, 1e-9, "PiecewiseFunction Evaluate inside first piece");
        AssertThrowsExactly<std::out_of_range>(
            [&function]() { function.Evaluate(-1.0); },
            "PiecewiseFunction Evaluate outside domain must throw out_of_range");

        function.Define(Interval<double>(3.0, 7.0), exp_function);
        AssertEqual(function.GetPieceCount(), 3, "Redefining inner interval must split one piece into three");

        AssertNear(function.Evaluate(2.0), 17.0, 1e-9, "Left remainder value");
        AssertNear(function.Evaluate(3.0), std::exp(3.0), 1e-9, "Shared left boundary must belong to newer piece");
        AssertNear(function.Evaluate(6.0), std::exp(6.0), 1e-9, "Middle replacement value");
        AssertNear(function.Evaluate(7.0), 42.0, 1e-9, "Shared right boundary must belong to older right remainder");
        AssertNear(function.Evaluate(9.0), 52.0, 1e-9, "Right remainder value");
    }

    void TestPiecewiseFunctionDefineOutsideRange()
    {
        PiecewiseFunction<double, double> function;
        LinearFunction<double> linear;
        ExpFunction<double> exp_function;
        PolynomialFunction<double> polynomial;

        function.Define(Interval<double>(0.0, 1.0), linear);
        function.Define(Interval<double>(2.0, 3.0), exp_function);
        function.Define(Interval<double>(-3.0, -2.0), polynomial);

        AssertEqual(function.GetPieceCount(), 3, "Disjoint Define calls must append three pieces");
        AssertNear(function.Evaluate(-2.5), polynomial.Evaluate(-2.5), 1e-9, "Left insertion must be preserved");
        AssertNear(function.Evaluate(0.5), 9.5, 1e-9, "Middle piece value");
        AssertNear(function.Evaluate(2.5), std::exp(2.5), 1e-9, "Right insertion must be preserved");
        AssertThrowsExactly<std::out_of_range>(
            [&function]() { function.Evaluate(1.5); },
            "Gap between disjoint pieces must remain outside domain");
    }

    void TestPiecewiseFunctionDefineBoundaryTouching()
    {
        PiecewiseFunction<double, double> function;
        LinearFunction<double> linear;
        ExpFunction<double> exp_function;

        function.Define(Interval<double>(0.0, 1.0), linear);
        function.Define(Interval<double>(1.0, 2.0), exp_function);

        AssertEqual(function.GetPieceCount(), 2, "Boundary-touching Define calls must keep two pieces");
        AssertNear(function.Evaluate(0.5), 9.5, 1e-9, "Left touching piece value");
        AssertNear(function.Evaluate(1.0), std::exp(1.0), 1e-9, "Shared boundary must belong to right piece");
        AssertNear(function.Evaluate(1.5), std::exp(1.5), 1e-9, "Right touching piece value");
    }

    void TestPiecewiseFunctionDefineFullReplacement()
    {
        PiecewiseFunction<double, double> function;
        LinearFunction<double> linear;
        ExpFunction<double> exp_function;

        function.Define(Interval<double>(0.0, 10.0), linear);
        function.Define(Interval<double>(0.0, 10.0), exp_function);

        AssertEqual(function.GetPieceCount(), 1, "Full replacement must leave a single piece");
        AssertNear(function.Evaluate(0.0), 1.0, 1e-9, "Full replacement must override left boundary");
        AssertNear(function.Evaluate(10.0), std::exp(10.0), 1e-9, "Full replacement must override right boundary");
    }

    void TestPiecewiseFunctionDefineAcrossMultiplePieces()
    {
        PiecewiseFunction<double, double> function;
        LinearFunction<double> linear;
        ExpFunction<double> exp_function;
        PolynomialFunction<double> polynomial;
        InverseSquareFunction<double> inverse_square;

        function.Define(Interval<double>(0.0, 2.0), linear);
        function.Define(Interval<double>(2.0, 4.0), exp_function);
        function.Define(Interval<double>(4.0, 6.0), polynomial);
        function.Define(Interval<double>(1.0, 5.0), inverse_square);

        AssertEqual(function.GetPieceCount(), 3, "Wide replacement across multiple pieces must leave two remainders and one replacement");
        AssertNear(function.Evaluate(0.5), 9.5, 1e-9, "Left remainder before multi-piece replacement");
        AssertNear(function.Evaluate(1.0), 1.0, 1e-9, "Replacement must own shared left boundary");
        AssertNear(function.Evaluate(3.0), 1.0 / 9.0, 1e-9, "Replacement interior value across removed pieces");
        AssertNear(function.Evaluate(5.0), polynomial.Evaluate(5.0), 1e-9, "Right remainder must own shared right boundary");
        AssertNear(function.Evaluate(5.5), polynomial.Evaluate(5.5), 1e-9, "Right remainder after multi-piece replacement");
    }

    void TestPiecewiseFunctionContinuity()
    {
        SinFunction<double> sin_function;
        PolynomialFunction<double> polynomial;
        LinearFunction<double> linear;
        ExpFunction<double> exp_function;

        PiecewiseFunction<double, double> continuous_function;
        continuous_function.Define(Interval<double>(-1.0, 0.0), sin_function);
        continuous_function.Define(Interval<double>(0.0, 1.0), polynomial);
        AssertTrue(continuous_function.IsContinuousOn(Interval<double>(-1.0, 1.0)), "Matching pieces must be continuous");
        AssertTrue(continuous_function.IsContinuousOn(Interval<double>(0.0, 0.0)), "Covered point interval must be continuous");

        PiecewiseFunction<double, double> discontinuous_function;
        discontinuous_function.Define(Interval<double>(-1.0, 0.0), linear);
        discontinuous_function.Define(Interval<double>(0.0, 1.0), exp_function);
        AssertTrue(!discontinuous_function.IsContinuousOn(Interval<double>(-1.0, 1.0)), "Jump at junction must break continuity");

        PiecewiseFunction<double, double> gap_function;
        gap_function.Define(Interval<double>(0.0, 1.0), linear);
        gap_function.Define(Interval<double>(2.0, 3.0), exp_function);
        AssertTrue(!gap_function.IsContinuousOn(Interval<double>(0.0, 3.0)), "Gap must break continuity");
    }

    void TestPiecewiseFunctionMonotonicity()
    {
        ExpFunction<double> exp_function;
        LinearFunction<double> linear;
        InverseSquareFunction<double> inverse_square;

        PiecewiseFunction<double, double> increasing_function;
        increasing_function.Define(Interval<double>(0.0, 1.0), exp_function);
        increasing_function.Define(Interval<double>(1.0, 2.0), linear);
        AssertEqual(
            increasing_function.GetMonotonicityOn(Interval<double>(0.0, 2.0)),
            Monotonicity::Increasing,
            "Upward junction between increasing pieces must stay increasing");
        AssertEqual(
            increasing_function.GetMonotonicityOn(Interval<double>(1.0, 1.0)),
            Monotonicity::Constant,
            "Point interval monotonicity must be constant");

        PiecewiseFunction<double, double> decreasing_function;
        decreasing_function.Define(Interval<double>(1.0, 2.0), inverse_square);
        decreasing_function.Define(Interval<double>(2.0, 4.0), inverse_square);
        AssertEqual(
            decreasing_function.GetMonotonicityOn(Interval<double>(1.0, 4.0)),
            Monotonicity::Decreasing,
            "Matching inverse square pieces must be decreasing");

        PiecewiseFunction<double, double> mixed_function;
        mixed_function.Define(Interval<double>(0.0, 1.0), linear);
        mixed_function.Define(Interval<double>(1.0, 2.0), inverse_square);
        AssertEqual(
            mixed_function.GetMonotonicityOn(Interval<double>(0.0, 2.0)),
            Monotonicity::NotMonotone,
            "Conflicting local monotonicity must be not monotone");

        PiecewiseFunction<double, double> gap_function;
        gap_function.Define(Interval<double>(0.0, 1.0), linear);
        gap_function.Define(Interval<double>(2.0, 3.0), exp_function);
        AssertEqual(
            gap_function.GetMonotonicityOn(Interval<double>(0.0, 3.0)),
            Monotonicity::NotMonotone,
            "Gap must be not monotone");
    }
}

void RunAllTests()
{
    TestInterval();
    TestBaseFunctions();
    TestPiece();
    TestPiecewiseFunctionEmptyState();
    TestPiecewiseFunctionIsDefinedAt();
    TestPiecewiseFunctionDefineInsideSinglePiece();
    TestPiecewiseFunctionDefineOutsideRange();
    TestPiecewiseFunctionDefineBoundaryTouching();
    TestPiecewiseFunctionDefineFullReplacement();
    TestPiecewiseFunctionDefineAcrossMultiplePieces();
    TestPiecewiseFunctionContinuity();
    TestPiecewiseFunctionMonotonicity();

    std::cout << "All tests passed." << std::endl;
}
