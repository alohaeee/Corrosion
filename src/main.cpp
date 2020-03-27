#include "corrosion.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QTextCodec>
#include <QTextStream>
#include <Windows.h>

using namespace corrosion;
int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{

    try
    {
        Compiler compiler;
        if (CORROSION_DEBUG_LOOP)
        {
            compiler.Simulate();
        }
        else
        {
            compiler.Command(argc, argv);
        }

        return 0;
    }
    catch (CException &e)
    {
        e.Log();
        return e.Error();
    }
}
