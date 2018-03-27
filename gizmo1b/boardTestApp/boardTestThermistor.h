#ifndef _BOARD_TEST_THERMISTOR_H_
#define _BOARD_TEST_THERMISTOR_H_

#include "boardTest.h"
#include "libThermistor.h"

class BoardTestThermistor: public BoardTest
{
public:
    BoardTestThermistor();
    virtual ~BoardTestThermistor();
    virtual int get(uint32 address, uint32& value);
private:
    LibThermistor m_libThermistor;
    int m_status;
};

#endif // _BOARD_TEST_THERMISTOR_H_
