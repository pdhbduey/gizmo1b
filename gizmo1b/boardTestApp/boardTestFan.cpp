#include "boardTestFan.h"

BoardTestFan::BoardTestFan()
{
}

BoardTestFan::~BoardTestFan()
{
}

int BoardTestFan::get(uint32 address, uint32& value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case FAN_VALUE:
        value = m_libFan.read();
        break;
    }
    return OKAY;
}

int BoardTestFan::set(uint32 address, uint32 value)
{
    switch (address) {
    default:
        return ERROR_ADDR;
    case FAN_VALUE:
        m_libFan.set(value);
        break;
    }
    return OKAY;
}
