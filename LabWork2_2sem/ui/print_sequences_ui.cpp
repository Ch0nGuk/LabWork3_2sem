#include "print_sequences_ui.h"

#include "sequence_ui_utils.h"

void PrintSequencesInUi(const UiState& state)
{
    PrintStoredSequences(state);
    PrintStoredDoubleSequences(state);
    PrintStoredPairSequences(state);
}
