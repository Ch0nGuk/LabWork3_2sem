#include "reduce_ui.h"

#include <iostream>

#include "sequence_ui_utils.h"

namespace
{
    int SumValues(int left, int right)
    {
        return left + right;
    }

    int MultiplyValues(int left, int right)
    {
        return left * right;
    }

    int MaxValue(int left, int right)
    {
        return left > right ? left : right;
    }
}

void ReduceInUi(const UiState& state)
{
    int index = 0;
    if (!SelectSequenceIndex(state, "Select sequence: ", index))
    {
        return;
    }

    std::cout << "Select reducer:\n";
    std::cout << "1. Sum\n";
    std::cout << "2. Product\n";
    std::cout << "3. Max\n";

    int choice = 0;
    if (!ReadInt("Choice: ", choice))
    {
        return;
    }

    int (*reduce_func)(int, int) = nullptr;

    switch (choice)
    {
    case 1:
        reduce_func = SumValues;
        break;
    case 2:
        reduce_func = MultiplyValues;
        break;
    case 3:
        reduce_func = MaxValue;
        break;
    default:
        std::cout << "Unknown reducer.\n";
        return;
    }

    int start_value = 0;
    if (!ReadInt("Write start value: ", start_value))
    {
        return;
    }

    int result = state.sequences[index]->Reduce(reduce_func, start_value);
    std::cout << "Reduce result: " << result << "\n";
}
