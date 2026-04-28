#include "ui.h"

#include <iostream>
#include <limits>

#include "append_ui.h"
#include "concat_ui.h"
#include "create_sequence_ui.h"
#include "insert_at_ui.h"
#include "map_ui.h"
#include "prepend_ui.h"
#include "print_sequences_ui.h"
#include "reduce_ui.h"
#include "run_tests_ui.h"
#include "slice_ui.h"
#include "subsequence_ui.h"
#include "ui_state.h"
#include "unzip_ui.h"
#include "where_ui.h"
#include "zip_ui.h"

void RunUi()
{
    UiState state;
    bool should_exit = false;

    while (!should_exit)
    {
        std::cout << "\n-------- MENU of Sequence --------\n\n";
        std::cout << "1.  Create sequence\n";
        std::cout << "2.  Print sequences\n";
        std::cout << "3.  Append item\n";
        std::cout << "4.  Prepend item\n";
        std::cout << "5.  Insert item at index\n";
        std::cout << "6.  Get subsequence\n";
        std::cout << "7.  Concat sequences\n";
        std::cout << "8.  Map sequence\n";
        std::cout << "9.  Where sequence\n";
        std::cout << "10. Reduce sequence\n";
        std::cout << "11. Slice sequence\n";
        std::cout << "12. Zip sequences\n";
        std::cout << "13. Unzip sequence\n";
        std::cout << "14. Run tests\n";
        std::cout << "0. Exit\n";
        std::cout << "\nChoice: ";

        int choice = 0;
        std::cin >> choice;

        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            continue;
        }

        switch (choice)
        {
        case 1:
            CreateSequenceInUi(state);
            break;
        case 2:
            PrintSequencesInUi(state);
            break;
        case 3:
            AppendInUi(state);
            break;
        case 4:
            PrependInUi(state);
            break;
        case 5:
            InsertAtInUi(state);
            break;
        case 6:
            GetSubsequenceInUi(state);
            break;
        case 7:
            ConcatInUi(state);
            break;
        case 8:
            MapInUi(state);
            break;
        case 9:
            WhereInUi(state);
            break;
        case 10:
            ReduceInUi(state);
            break;
        case 11:
            SliceInUi(state);
            break;
        case 12:
            ZipInUi(state);
            break;
        case 13:
            UnzipInUi(state);
            break;
        case 14:
            RunTestsInUi();
            break;
        case 0:
            should_exit = true;
            break;
        default:
            std::cout << "Unknown action.\n";
            break;
        }
    }

    for (Sequence<int>* sequence : state.sequences)
    {
        delete sequence;
    }

    for (Sequence<double>* sequence : state.double_sequences)
    {
        delete sequence;
    }

    for (Sequence<std::pair<int, int>>* sequence : state.pair_sequences)
    {
        delete sequence;
    }
}
