#ifndef UI_STATE_H
#define UI_STATE_H

#include <vector>

#include "../PiecewiseFunction.h"

struct UiState
{
    std::vector<PiecewiseFunction<double, double>> functions;
};

#endif // UI_STATE_H
