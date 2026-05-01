#ifndef PIECEWISE_UI_UTILS_H
#define PIECEWISE_UI_UTILS_H

#include <string>

#include "../IFunction.h"
#include "../Interval.h"
#include "../Monotonicity.h"
#include "../PiecewiseFunction.h"
#include "ui_state.h"

bool ReadInt(const std::string& prompt, int& value);
bool ReadDouble(const std::string& prompt, double& value);
bool ReadInterval(Interval<double>& interval);

void PrintStoredFunctions(const UiState& state);
void PrintFunction(const PiecewiseFunction<double, double>& function);
bool SelectFunctionIndex(const UiState& state, const std::string& prompt, int& index);

void ConfigureFloatingPointOutput();
const char* FunctionName(const IFunction<double, double>& function);
const char* MonotonicityToString(Monotonicity monotonicity);

#endif // PIECEWISE_UI_UTILS_H
