#pragma once

#include <BoardTestApp1.h>

class BoardTestApp2: public BoardTestApp1 {
public:
    BoardTestApp2(const char* name);
    virtual void setupTests();
};
