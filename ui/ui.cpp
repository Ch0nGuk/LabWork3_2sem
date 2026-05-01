#include "ui.h"

#include <iostream>

#include "continuity_ui.h"
#include "create_function_ui.h"
#include "define_piece_ui.h"
#include "evaluate_ui.h"
#include "monotonicity_ui.h"
#include "piecewise_ui_utils.h"
#include "print_functions_ui.h"
#include "run_tests_ui.h"
#include "ui_state.h"

namespace
{
    void PrintMenu()
    {
        std::cout << "\n-------- MENU of Piecewise Function --------\n\n";
        std::cout << "1. Create piecewise function\n";
        std::cout << "2. Print functions\n";
        std::cout << "3. Define/redefine interval\n";
        std::cout << "4. Evaluate at point\n";
        std::cout << "5. Check monotonicity\n";
        std::cout << "6. Check continuity\n";
        std::cout << "7. Run tests\n";
        std::cout << "0. Exit\n";
    }
}

void RunUi()
{
    ConfigureFloatingPointOutput();

    UiState state;
    bool should_exit = false;

    while (!should_exit)
    {
        PrintMenu();

        int choice = 0;
        if (!ReadInt("\nChoice: ", choice))
        {
            if (std::cin.eof())
            {
                break;
            }

            continue;
        }

        switch (choice)
        {
        case 1:
            CreateFunctionInUi(state);
            break;
        case 2:
            PrintFunctionsInUi(state);
            break;
        case 3:
            DefinePieceInUi(state);
            break;
        case 4:
            EvaluateInUi(state);
            break;
        case 5:
            CheckMonotonicityInUi(state);
            break;
        case 6:
            CheckContinuityInUi(state);
            break;
        case 7:
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
}
