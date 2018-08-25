#include <BoardTestConsoleApp1.h>

BoardTestConsoleApp1::BoardTestConsoleApp1(const char* name) :
    BoardTestConsoleApp(this),
    BoardTestApp1(name)
{
}

void BoardTestConsoleApp1::run()
{
    setupTests();
    msgLoop();
}
