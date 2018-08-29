#pragma once

#include <LibAdc.h>
#include <LibDac.h>
#include <LibThermistor.h>
#include <LibTec.h>

class LibTec2 : public LibTec
{
public:
    LibTec2(LibDac* libDac, LibAdc* libAdc, LibThermistor* libThermistor, const char* name = "LibTec2");
    virtual float toIsense(float voltage);
    virtual float toVsense(float voltage);
};
