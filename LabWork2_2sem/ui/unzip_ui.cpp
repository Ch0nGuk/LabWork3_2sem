#include "unzip_ui.h"

#include <iostream>
#include <memory>
#include <utility>

#include "../sequence_factory.h"
#include "sequence_ui_utils.h"
#include "../unzip.h"

void UnzipInUi(UiState& state)
{
    int index = 0;
    if (!SelectPairSequenceIndex(state, "Select pair sequence: ", index))
    {
        return;
    }

    ListSequenceFactory<int> factory;
    std::pair<Sequence<int>*, Sequence<int>*> raw_result =
        Unzip<int, int>(*state.pair_sequences[index], factory, factory);
    std::unique_ptr<Sequence<int>> first_result(raw_result.first);
    std::unique_ptr<Sequence<int>> second_result(raw_result.second);

    state.sequences.push_back(first_result.get());
    state.sequences.push_back(second_result.get());

    std::cout << "Created first sequence: ";
    PrintSequence(first_result.get());
    std::cout << "\n";

    std::cout << "Created second sequence: ";
    PrintSequence(second_result.get());
    std::cout << "\n";

    first_result.release();
    second_result.release();
}
