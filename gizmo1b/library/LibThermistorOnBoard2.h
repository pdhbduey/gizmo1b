#pragma once

#include <LibAdc.h>
#include <LibThermistor.h>
#include <LibThermistorCurves.h>

class LibThermistorOnBoard2 : public LibThermistor
{
public:
    LibThermistorOnBoard2(LibAdc* libAdc);
private:
    // LibThermistor
    virtual float convertVoltageToResistance(float ain);
};
