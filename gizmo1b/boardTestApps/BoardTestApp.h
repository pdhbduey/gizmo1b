#pragma once

#include <vector>
#include <map>
#include <sys_common.h>
#include <BoardTest.h>

class BoardTestApp {
public:
    BoardTestApp();
    int regRead(uint32 address, uint32& value);
    int regWrite(uint32 address, uint32 value);
    virtual void decodeMessage(std::vector<uint8>& message,
                                              std::vector<uint8>& response) = 0;
    virtual void setupTests() = 0;
private:
    bool isAddressValid(uint32 address);
protected:
    std::map<int, BoardTest*> m_boardTestMap;
};
