#include "create_sequence_ui.h"

#include <iostream>
#include <memory>
#include <vector>

#include "../ImmutableArraySequence.h"
#include "../ListSequence.h"
#include "../MutableArraySequence.h"
#include "sequence_ui_utils.h"

void CreateSequenceInUi(UiState& state)
{
    std::cout << "Select sequence type:\n";
    std::cout << "1. MutableArraySequence\n";
    std::cout << "2. ImmutableArraySequence\n";
    std::cout << "3. ListSequence\n";

    int sequence_type = 0;
    if (!ReadInt("Choice: ", sequence_type))
    {
        return;
    }

    int length = 0;
    if (!ReadInt("Write sequence length: ", length) || length < 0)
    {
        std::cout << "Invalid length.\n";
        return;
    }

    std::vector<int> items(length);
    for (int index = 0; index < length; index++)
    {
        if (!ReadInt("Write item #" + std::to_string(index) + ": ", items[index]))
        {
            return;
        }
    }

    std::unique_ptr<Sequence<int>> created_sequence;

    switch (sequence_type)
    {
    case 1:
        created_sequence = std::make_unique<MutableArraySequence<int>>(items.data(), length);
        break;
    case 2:
        created_sequence = std::make_unique<ImmutableArraySequence<int>>(items.data(), length);
        break;
    case 3:
        created_sequence = std::make_unique<ListSequence<int>>(items.data(), length);
        break;
    default:
        std::cout << "Unknown sequence type.\n";
        return;
    }

    state.sequences.push_back(created_sequence.get());
    std::cout << "Sequence created: ";
    PrintSequence(created_sequence.get());
    std::cout << "\n";
    created_sequence.release();
}
