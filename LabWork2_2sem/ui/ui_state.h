#ifndef UI_STATE_H
#define UI_STATE_H

#include <vector>
#include <utility>

#include "../Sequence.h"

struct UiState
{
    std::vector<Sequence<int>*> sequences;
    std::vector<Sequence<double>*> double_sequences;
    std::vector<Sequence<std::pair<int, int>>*> pair_sequences;
};

#endif // UI_STATE_H
