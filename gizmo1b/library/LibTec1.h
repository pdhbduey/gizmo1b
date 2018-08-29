#pragma once

#include <LibTec.h>
#include <LibAdc.h>
#include <LibDac.h>
#include <LibThermistor.h>

class LibTec1 : public LibTec
{
public:
    LibTec1(LibDac* libDac, LibAdc* libAdc, LibThermistor* libThermistor, const char* name = "LibTec1");
    virtual float toIsense(float voltage);
    virtual float toVsense(float voltage);
};
