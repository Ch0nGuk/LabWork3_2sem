#include "run_tests_ui.h"

#include <exception>
#include <iostream>

#include "../tests.h"

void RunTestsInUi()
{
    try
    {
        RunAllTests();
    }
    catch (const std::exception& error)
    {
        std::cout << "Tests failed: " << error.what() << "\n";
    }
}
