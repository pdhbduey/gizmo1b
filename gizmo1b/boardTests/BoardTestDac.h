#pragma once

#include <LibDac.h>
#include <BoardTest.h>
#include <LibTec.h>

class BoardTestDac: public BoardTest
{
public:
    BoardTestDac(LibDac* libDac, LibTec* libTec);
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibDac& m_libDac;
    LibTec& m_libTec;
    int m_status;
};
