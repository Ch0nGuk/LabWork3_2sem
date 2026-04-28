#include "sequence_ui_utils.h"

#include <iostream>
#include <limits>
#include <utility>

bool ReadInt(const std::string& prompt, int& value)
{
    std::cout << prompt;
    std::cin >> value;

    if (!std::cin)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input.\n";
        return false;
    }

    return true;
}

void PrintSequence(const Sequence<int>* sequence)
{
    std::cout << "[";

    for (int index = 0; index < sequence->GetLength(); index++)
    {
        if (index > 0)
        {
            std::cout << ", ";
        }

        std::cout << sequence->Get(index);
    }

    std::cout << "]";
}

void PrintSequence(const Sequence<double>* sequence)
{
    std::cout << "[";

    for (int index = 0; index < sequence->GetLength(); index++)
    {
        if (index > 0)
        {
            std::cout << ", ";
        }

        std::cout << sequence->Get(index);
    }

    std::cout << "]";
}

void PrintSequence(const Sequence<std::pair<int, int>>* sequence)
{
    std::cout << "[";

    for (int index = 0; index < sequence->GetLength(); index++)
    {
        if (index > 0)
        {
            std::cout << ", ";
        }

        const std::pair<int, int>& value = sequence->Get(index);
        std::cout << "(" << value.first << ", " << value.second << ")";
    }

    std::cout << "]";
}

void PrintStoredSequences(const UiState& state)
{
    if (state.sequences.empty())
    {
        std::cout << "No sequences created.\n";
        return;
    }

    std::cout << "Stored sequences:\n";

    for (std::size_t index = 0; index < state.sequences.size(); index++)
    {
        std::cout << index + 1 << ". ";
        PrintSequence(state.sequences[index]);
        std::cout << " (length = " << state.sequences[index]->GetLength() << ")\n";
    }
}

void PrintStoredDoubleSequences(const UiState& state)
{
    if (state.double_sequences.empty())
    {
        std::cout << "No double sequences created.\n";
        return;
    }

    std::cout << "Stored double sequences:\n";

    for (std::size_t index = 0; index < state.double_sequences.size(); index++)
    {
        std::cout << index + 1 << ". ";
        PrintSequence(state.double_sequences[index]);
        std::cout << " (length = " << state.double_sequences[index]->GetLength() << ")\n";
    }
}

void PrintStoredPairSequences(const UiState& state)
{
    if (state.pair_sequences.empty())
    {
        std::cout << "No pair sequences created.\n";
        return;
    }

    std::cout << "Stored pair sequences:\n";

    for (std::size_t index = 0; index < state.pair_sequences.size(); index++)
    {
        std::cout << index + 1 << ". ";
        PrintSequence(state.pair_sequences[index]);
        std::cout << " (length = " << state.pair_sequences[index]->GetLength() << ")\n";
    }
}

bool SelectSequenceIndex(const UiState& state, const std::string& prompt, int& index)
{
    if (state.sequences.empty())
    {
        std::cout << "No sequences available.\n";
        return false;
    }

    PrintStoredSequences(state);

    int choice = 0;
    if (!ReadInt(prompt, choice))
    {
        return false;
    }

    if (choice <= 0 || choice > static_cast<int>(state.sequences.size()))
    {
        std::cout << "Wrong number.\n";
        return false;
    }

    index = choice - 1;
    return true;
}

bool SelectDoubleSequenceIndex(const UiState& state, const std::string& prompt, int& index)
{
    if (state.double_sequences.empty())
    {
        std::cout << "No double sequences available.\n";
        return false;
    }

    PrintStoredDoubleSequences(state);

    int choice = 0;
    if (!ReadInt(prompt, choice))
    {
        return false;
    }

    if (choice <= 0 || choice > static_cast<int>(state.double_sequences.size()))
    {
        std::cout << "Wrong number.\n";
        return false;
    }

    index = choice - 1;
    return true;
}

bool SelectPairSequenceIndex(const UiState& state, const std::string& prompt, int& index)
{
    if (state.pair_sequences.empty())
    {
        std::cout << "No pair sequences available.\n";
        return false;
    }

    PrintStoredPairSequences(state);

    int choice = 0;
    if (!ReadInt(prompt, choice))
    {
        return false;
    }

    if (choice <= 0 || choice > static_cast<int>(state.pair_sequences.size()))
    {
        std::cout << "Wrong number.\n";
        return false;
    }

    index = choice - 1;
    return true;
}
