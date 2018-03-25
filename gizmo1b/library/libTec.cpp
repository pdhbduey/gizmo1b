#include "mibspi.h"
#include "libWrapMibSpi5.h"
#include "libTec.h"

LibTec::LibTec() :
    m_tecEnable(new LibWrapMibSpi5, PIN_SIMO) // 99:MIBSPI5SIMO[0]:TEC_EN
{
    enable(false);
}

LibTec::~LibTec()
{
}

void LibTec::enable(bool en)
{
    m_tecEnable.m_libWrapGioPort->setPin(m_tecEnable.m_pin, en);
}

float LibTec::getISense()
{
    return 0.0;
}

float LibTec::getVSense()
{
    return 0.0;
}

void LibTec::setCurrent(float value)
{
}

void LibTec::setCurrent(int loopCount, std::vector<TimeCurrent>& setPoints)
{
}

void LibTec::stopCurrent()
{
}

int LibTec::getLoopValue()
{
    return 0;
}

int LibTec::getCountValue()
{
    return 0;
}
