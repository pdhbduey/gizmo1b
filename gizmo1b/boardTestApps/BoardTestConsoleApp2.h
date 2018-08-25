#pragma once

#include <vector>
#include <BoardTestApp2.h>
#include <BoardTestConsoleApp.h>

class BoardTestConsoleApp2: public BoardTestApp2, public BoardTestConsoleApp {
public:
    BoardTestConsoleApp2(const char* name);
private:
    // BoardTestApp1
    virtual void run();
    // BoardTestConsoleApp
    virtual bool parseCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
    virtual bool parseHelpCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
    bool parseAdcIVCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
    bool adcIVHelp(std::vector<std::string>& tokens, std::string& res);
private:
    std::string m_prevMsg;
};
