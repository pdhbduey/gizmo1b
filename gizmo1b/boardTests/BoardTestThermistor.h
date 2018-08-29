#pragma once

#include <LibThermistor.h>
#include <BoardTest.h>

class BoardTestThermistor: public BoardTest
{
public:
    enum Type {
        SC30F103AN = 1 << LibThermistorCurves::SC30F103AN,
        USP12837   = 1 << LibThermistorCurves::USP12837,
    };
public:
    BoardTestThermistor(LibThermistor* libThermistor, LibThermistor* libThermistorOnBoard = 0);
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibThermistor& m_libThermistor;
    LibThermistor* m_libThermistorOnBoard;
    int m_status;
};
