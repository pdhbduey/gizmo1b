#pragma once

#include <LibDac.h>
#include <BoardTest.h>

class BoardTestDac: public BoardTest
{
public:
    BoardTestDac(LibDac* libDac);
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibDac& m_libDac;
    int m_status;
};
