#include <BoardTestApp.h>
#include <mibspi.h>
#include <gio.h>
#include <sci.h>
#include <het.h>

BoardTestApp::BoardTestApp()
{
    mibspiInit();
    hetInit();
    gioInit();
    sciInit();
}

int BoardTestApp::regRead(uint32 address, uint32& value)
{
    if (isAddressValid(address)) {
        return m_boardTestMap[address]->get(address, value);
    }
    return BoardTest::ERROR_ADDR;
}

int BoardTestApp::regWrite(uint32 address, uint32 value)
{
    if (isAddressValid(address)) {
        return m_boardTestMap[address]->set(address, value);
    }
    return BoardTest::ERROR_ADDR;
}

bool BoardTestApp::isAddressValid(uint32 address)
{
    return (m_boardTestMap.find(address) != m_boardTestMap.end()
        && m_boardTestMap[address]);
}
