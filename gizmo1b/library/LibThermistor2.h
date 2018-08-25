#pragma once

#include <LibAdc.h>
#include <LibThermistor.h>
#include <LibThermistorCurves.h>

class LibThermistor2 : public LibThermistor
{
public:
    LibThermistor2(LibAdc* libAdc,
                   int type  = LibThermistorCurves::SC30F103AN,
                   int units = LibThermistorCurves::CELSIUS);
private:
    // LibThermistor
    virtual float convertVoltageToResistance(float ain);
};
