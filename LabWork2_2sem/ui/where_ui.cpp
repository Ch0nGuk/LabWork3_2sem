#include "where_ui.h"

#include <iostream>
#include <memory>

#include "sequence_ui_utils.h"

namespace
{
    bool IsEvenValue(int value)
    {
        return value % 2 == 0;
    }

    bool IsOddValue(int value)
    {
        return value % 2 != 0;
    }

    bool IsPositiveValue(int value)
    {
        return value > 0;
    }

    bool IsNegativeValue(int value)
    {
        return value < 0;
    }
}

void WhereInUi(UiState& state)
{
    int index = 0;
    if (!SelectSequenceIndex(state, "Select sequence: ", index))
    {
        return;
    }

    std::cout << "Select predicate:\n";
    std::cout << "1. Is even\n";
    std::cout << "2. Is odd\n";
    std::cout << "3. Is positive\n";
    std::cout << "4. Is negative\n";

    int choice = 0;
    if (!ReadInt("Choice: ", choice))
    {
        return;
    }

    bool (*predicate)(int) = nullptr;

    switch (choice)
    {
    case 1:
        predicate = IsEvenValue;
        break;
    case 2:
        predicate = IsOddValue;
        break;
    case 3:
        predicate = IsPositiveValue;
        break;
    case 4:
        predicate = IsNegativeValue;
        break;
    default:
        std::cout << "Unknown predicate.\n";
        return;
    }

    std::unique_ptr<Sequence<int>> result(state.sequences[index]->Where(predicate));
    state.sequences.push_back(result.get());

    std::cout << "Created filtered sequence: ";
    PrintSequence(result.get());
    std::cout << "\n";
    result.release();
}
