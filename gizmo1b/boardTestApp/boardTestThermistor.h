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
    virtual int set(uint32 address, uint32 value);
private:
    enum BitsAdcControlMask {
        THERMISTOR_CHANNEL_SELECT_MASK = (uint32)((uint32)7U << 0U),
        START_READING_MASK             = (uint32)((uint32)1U << 8U),
    };
    enum BitsThermistorControlShift {
        THERMISTOR_CHANNEL_SELECT_SHIFT = 0,
        START_READING_SHIFT             = 8,
    };
    enum BitsThermistorStatusMask {
        THERMISTOR_STATUS_MASK = (uint32)((uint32)3U << 0U),
    };
    enum BitsThermistorStatusShift {
        THERMISTOR_STATUS_SHIFT = 0,
    };
    LibThermistor m_libThermistor;
    int m_channel;
};

#endif // _BOARD_TEST_THERMISTOR_H_
