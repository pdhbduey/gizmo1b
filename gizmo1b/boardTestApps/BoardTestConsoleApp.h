#pragma once

#include <vector>
#include <string>
#include <sys_common.h>
#include <BoardTestApp.h>

class BoardTestConsoleApp {
public:
    enum Message {
        COMPONENT,
        ACTION,
        ARGUMENT,
        VALUE,
    };
public:
    BoardTestConsoleApp(BoardTestApp* boardTestApp);
    void msgLoop();
    virtual bool parseCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseHelpCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool dacHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool adcHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool ledHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool faultHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool tecHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool heaterHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool thermistorHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool motorHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool fanHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool dioHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool opticsHelp(std::vector<std::string>& tokens, std::string& res);
    virtual bool parseTecCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseHeaterCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseFaultCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseThermistorCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseLedCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseDacCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseAdcCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseMotorCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseFanCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseDioCommand(std::vector<std::string>& tokens, std::string& res, int& result);
    virtual bool parseOpticsCommand(std::vector<std::string>& tokens, std::string& res, int& result);
protected:
    void decodeMessage(std::vector<uint8>& message, std::vector<uint8>& response);
protected:
    std::string m_prevMsg;
    BoardTestApp& m_boardTestApp;
};
