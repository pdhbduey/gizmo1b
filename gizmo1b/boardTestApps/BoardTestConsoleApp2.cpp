#include <BoardTestConsoleApp2.h>
#include <string>
#include <vector>
#include <map>
#include <string>
#include <sys_common.h>
#include <BoardTestAdcIV.h>
#include <stdio.h>

BoardTestConsoleApp2::BoardTestConsoleApp2(const char* name) :
    BoardTestConsoleApp(this),
    BoardTestApp2(name)
{
}

void BoardTestConsoleApp2::run()
{
    setupTests();
    msgLoop();
}

bool BoardTestConsoleApp2::parseCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    if (tokens[COMPONENT] == "help") {
        parseHelpCommand(tokens, res);
        isParsingError = false;
    }
    else if (tokens[COMPONENT] == "fault") {
        isParsingError = parseFaultCommand(tokens, res, result);
    }
    else if (tokens[COMPONENT] == "thermistor") {
        isParsingError = parseThermistorCommand(tokens, res, result);
    }
    else if (tokens[COMPONENT] == "led") {
        isParsingError = parseLedCommand(tokens, res, result);
    }
    else if (tokens[COMPONENT] == "dac") {
        isParsingError = parseDacCommand(tokens, res, result);
    }
    else if (tokens[COMPONENT] == "adc") {
        isParsingError = parseAdcCommand(tokens, res, result);
    }
    else if (tokens[COMPONENT] == "adciv") {
        isParsingError = parseAdcIVCommand(tokens, res, result);
    }
    else if (tokens[COMPONENT] == "tec") {
        isParsingError = parseTecCommand(tokens, res, result);
    }
    return isParsingError;
}

void BoardTestConsoleApp2::parseHelpCommand(std::vector<std::string>& tokens,
                                                               std::string& res)
{
    if (tokens.size() > ACTION
         && (dacHelp       (tokens, res)
         ||  adcHelp       (tokens, res)
         ||  adcIVHelp     (tokens, res)
         ||  ledHelp       (tokens, res)
         ||  faultHelp     (tokens, res)
         ||  tecHelp       (tokens, res)
         ||  thermistorHelp(tokens, res))) {
    }
    else {
        res = "help dac|adc|adciv|led|fault|thermistor|tec";
    }
}

bool BoardTestConsoleApp2::adcIVHelp(std::vector<std::string>& tokens,
                                                               std::string& res)
{
    if (tokens[ACTION] == "adciv") {
        res += "adciv get 0..1|all\n\r";
        return true;
    }
    return false;
}

bool BoardTestConsoleApp2::parseAdcIVCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    std::vector<std::string> adcIVStatus;
    adcIVStatus.push_back("OKAY");
    adcIVStatus.push_back("ERROR_INVALID_CHANNEL");
    adcIVStatus.push_back("ERROR_TIME_OUT");
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "all") {
                for (int i = 0; i <= 1; i++) {
                    int channel = BoardTest::ADC_IV_CHANNEL_0 + i;
                    uint32 value;
                    result = m_boardTestApp.regRead(channel, value);
                    if (result == BoardTest::OKAY) {
                        float f = *reinterpret_cast<float*>(&value);
                        result = m_boardTestApp.regRead(BoardTest::ADC_IV_STATUS, value);
                        if (value != LibAdc::OKAY) {
                            res = "adciv status: " + adcIVStatus[value];
                            break;
                        }
                        else {
                            char t[32];
                            sprintf(t, "[%d]:%.2fV ", i, f);
                            res += t;
                        }
                    }
                }
                isParsingError = false;
            }
            else {
                int adcIVChannel;
                if (sscanf(tokens[ARGUMENT].c_str(), "%d", &adcIVChannel) == 1
                && (adcIVChannel == 0
                ||  adcIVChannel == 1)) {
                    int channel = BoardTest::ADC_IV_CHANNEL_0 + adcIVChannel;
                    uint32 value;
                    result = m_boardTestApp.regRead(channel, value);
                    if (result == BoardTest::OKAY) {
                        float f = *reinterpret_cast<float*>(&value);
                        result = m_boardTestApp.regRead(BoardTest::ADC_IV_STATUS, value);
                        if (value != LibAdc::OKAY) {
                            res = "adivc status: " + adcIVStatus[value];
                        }
                        else {
                            char t[16];
                            sprintf(t, "%.2fV", f);
                            res += t;
                        }
                    }
                    isParsingError = false;
                }
            }
        }
    }
    return isParsingError;
}
