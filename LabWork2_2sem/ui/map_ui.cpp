#include "map_ui.h"

#include <iostream>
#include <memory>

#include "../map.h"
#include "sequence_ui_utils.h"
#include "../sequence_factory.h"

namespace
{
    int Identity(int value)
    {
        return value;
    }

    int DoubleValue(int value)
    {
        return value * 2;
    }

    int SquareValue(int value)
    {
        return value * value;
    }

    int NegateValue(int value)
    {
        return -value;
    }

    double HalfValue(int value)
    {
        return value / 2.0;
    }

    double ShiftValue(int value)
    {
        return value + 0.5;
    }
}

void MapInUi(UiState& state)
{
    int index = 0;
    if (!SelectSequenceIndex(state, "Select sequence: ", index))
    {
        return;
    }

    std::cout << "Select map function:\n";
    std::cout << "1. Identity\n";
    std::cout << "2. Double\n";
    std::cout << "3. Square\n";
    std::cout << "4. Negate\n";
    std::cout << "5. Half (double)\n";
    std::cout << "6. Shift by 0.5 (double)\n";

    int choice = 0;
    if (!ReadInt("Choice: ", choice))
    {
        return;
    }

    switch (choice)
    {
    case 1:
    {
        ListSequenceFactory<int> factory;
        std::unique_ptr<Sequence<int>> result(Map<int, int>(*state.sequences[index], Identity, factory));
        state.sequences.push_back(result.get());

        std::cout << "Created mapped sequence: ";
        PrintSequence(result.get());
        std::cout << "\n";
        result.release();
        break;
    }
    case 2:
    {
        ListSequenceFactory<int> factory;
        std::unique_ptr<Sequence<int>> result(Map<int, int>(*state.sequences[index], DoubleValue, factory));
        state.sequences.push_back(result.get());

        std::cout << "Created mapped sequence: ";
        PrintSequence(result.get());
        std::cout << "\n";
        result.release();
        break;
    }
    case 3:
    {
        ListSequenceFactory<int> factory;
        std::unique_ptr<Sequence<int>> result(Map<int, int>(*state.sequences[index], SquareValue, factory));
        state.sequences.push_back(result.get());

        std::cout << "Created mapped sequence: ";
        PrintSequence(result.get());
        std::cout << "\n";
        result.release();
        break;
    }
    case 4:
    {
        ListSequenceFactory<int> factory;
        std::unique_ptr<Sequence<int>> result(Map<int, int>(*state.sequences[index], NegateValue, factory));
        state.sequences.push_back(result.get());

        std::cout << "Created mapped sequence: ";
        PrintSequence(result.get());
        std::cout << "\n";
        result.release();
        break;
    }
    case 5:
    {
        ListSequenceFactory<double> factory;
        std::unique_ptr<Sequence<double>> result(Map<int, double>(*state.sequences[index], HalfValue, factory));
        state.double_sequences.push_back(result.get());

        std::cout << "Created mapped double sequence: ";
        PrintSequence(result.get());
        std::cout << "\n";
        result.release();
        break;
    }
    case 6:
    {
        ListSequenceFactory<double> factory;
        std::unique_ptr<Sequence<double>> result(Map<int, double>(*state.sequences[index], ShiftValue, factory));
        state.double_sequences.push_back(result.get());

        std::cout << "Created mapped double sequence: ";
        PrintSequence(result.get());
        std::cout << "\n";
        result.release();
        break;
    }
    default:
        std::cout << "Unknown map function.\n";
        return;
    }
}
