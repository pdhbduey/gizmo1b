#ifndef _BOARD_TEST_THERMISTOR_H_
#define _BOARD_TEST_THERMISTOR_H_

#include "boardTest.h"
#include "libThermistor.h"

class BoardTestThermistor: public BoardTest
{
public:
    enum Type {
        SC30F103AN = 1 << LibThermistor::SC30F103AN,
        USP12387   = 1 << LibThermistor::USP12387,
    };
public:
    BoardTestThermistor();
    virtual ~BoardTestThermistor();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibThermistor m_libThermistor;
    int m_status;
};

#endif // _BOARD_TEST_THERMISTOR_H_
