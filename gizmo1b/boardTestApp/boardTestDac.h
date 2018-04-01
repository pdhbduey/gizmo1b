#ifndef _BOARD_TEST_DAC_H_
#define _BOARD_TEST_DAC_H_

#include "boardTest.h"
#include "libDac.h"

class BoardTestDac: public BoardTest
{
public:
    BoardTestDac();
    virtual ~BoardTestDac();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibDac m_libDac;
    int m_status;
};

#endif // _BOARD_TEST_DAC_H_
