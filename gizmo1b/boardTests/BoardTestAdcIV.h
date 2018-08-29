#pragma once

#include <LibAdc.h>
#include <BoardTest.h>

class BoardTestAdcIV: public BoardTest
{
public:
    BoardTestAdcIV(LibAdc* libAdc);
    virtual int get(uint32 address, uint32& value);
private:
    LibAdc& m_libAdc;
    int m_status;
};
