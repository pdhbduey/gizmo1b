#ifndef _BOARD_TEST_CONSOLE_APP_H_
#define _BOARD_TEST_CONSOLE_APP_H_

#include <map>
#include "boardTestApp.h"

class BoardTestConsoleApp: public BoardTestApp {
public:
    BoardTestConsoleApp(const char* name);
    virtual ~BoardTestConsoleApp();
private:
    enum Message {
        COMPONENT,
        ACTION,
        ARGUMENT,
        VALUE,
    };
private:
    virtual void run();
    void decodeMessage(std::vector<uint8>& message,
                                                  std::vector<uint8>& response);
    void help(std::string& help);
    bool parseTecCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
    bool parseFaultCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
    bool parseThermistorCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
    bool parseLedCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
    bool parseDacCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
    bool parseAdcCommand(std::vector<std::string>& tokens,
                                                 std::string& res, int& result);
};

#endif // _BOARD_TEST_CONSOLE_APP_H_