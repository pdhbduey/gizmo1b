#ifndef _BOARD_TEST_FAN_H_
#define _BOARD_TEST_FAN_H_

#include "boardTest.h"
#include "libFan.h"

class BoardTestFan: public BoardTest
{
public:
    BoardTestFan();
    virtual ~BoardTestFan();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibFan m_libFan;
};

#endif // _BOARD_TEST_FAN_H_
