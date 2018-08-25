#pragma once

#include <vector>
#include <BoardTestApp1.h>
#include <BoardTestConsoleApp.h>

class BoardTestConsoleApp1: public BoardTestApp1, public BoardTestConsoleApp {
public:
    BoardTestConsoleApp1(const char* name);
private:
    // BoardTestApp1
    virtual void run();
};
