#include <string>
#include <vector>
#include <stdio.h>
#include <map>
#include "libSci2.h"
#include "boardTestTec.h"
#include "boardTestFault.h"
#include "boardTestLed.h"
#include "libDac.h"
#include "libAdc.h"
#include "libTec.h"
#include "boardTestAdc.h"
#include "boardTestConsoleApp.h"

BoardTestConsoleApp::BoardTestConsoleApp(const char* name) :
    BoardTestApp(name)
{
}

BoardTestConsoleApp::~BoardTestConsoleApp()
{
}

void BoardTestConsoleApp::run()
{
    LibSci2 libSci(1024, 1024);
    bool resetSci = true;
    std::vector<uint8> message;
    message.reserve(32);
    std::vector<uint8> response;
    response.reserve(32);
    while (true) {
        if (resetSci) {
            libSci.close();
            libSci.setBaudRate(LibSci::BAUD_115200);
            libSci.setParity(LibSci::NONE);
            libSci.setStopBits(LibSci::TWO);
            libSci.open();
            resetSci = false;
        }
        message.clear();
        while (message.size() < 1024) {
            if (libSci.isDataAvailable(1000)) {
                libSci.read(message);
                if (message.back() == '\r') {
                    break;
                }
            }
        }
        if (!resetSci) {
            decodeMessage(message, response);
            if (libSci.write(response) == LibSci::OKAY) {
                if (!libSci.isDataWritten(1000)) {
                    resetSci = true;
                }
            }
            else {
                resetSci = true;
            }
        }
    }
}

void BoardTestConsoleApp::help(std::string& help)
{
    help  = "ERROR: Unrecognized command\n\r";
    help += "USAGE:\n\r";
    help += "dac set [0,5]\n\r";
    help += "dac get\n\r";
    help += "adc get 0..5\n\r";
    help += "led set green|red on|off\n\r";
    help += "led get green|red\n\r";
    help += "fault reset\n\r";
    help += "tec enable|disable\n\r";
    help += "tec get isense|vsense|iref|waveformtype|waveformperiod|waveform\n\r";
    help += "tec set iref [-15,15]\n\r";
    help += "tec set waveformtype sin|tr|sq|const\n\r";
    help += "tec set waveformperiod 500..10,000ms\n\r";
    help += "tec set waveform start|stop\n\r";
    help += "thermistor get a|b|c|d\n\r";
}

void BoardTestConsoleApp::decodeMessage(std::vector<uint8>& message,
                                                   std::vector<uint8>& response)
{
    std::string msg;
    for (int i = 0; i < message.size(); i++) {
        if (message[i] == '\r' || message[i] == '\n') {
            continue;
        }
        msg += message[i];
    }
    bool isParsingError = true;
    std::string res;
    std::vector<std::string> status;
    status.push_back("OKAY");
    status.push_back("ERROR_CMD");
    status.push_back("ERROR_ADDR");
    status.push_back("ERROR_RO");
    status.push_back("ERROR_WO");
    int result;
    if (msg.size()) {
        size_t pos = 0;
        std::string token;
        std::vector<std::string> tokens;
        std::string delimiter(" ");
        while ((pos = msg.find(delimiter)) != std::string::npos) {
            token = msg.substr(0, pos);
            tokens.push_back(token);
            msg.erase(0, pos + delimiter.length());
        }
        if (msg.size()) {
            tokens.push_back(msg);
        }
        if (tokens.size() > COMPONENT) {
            if (tokens[COMPONENT] == "tec") {
                isParsingError = parseTecCommand(tokens, res, result);
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
        }
    }
    if (isParsingError) {
        help(res);
    }
    else {
        if (res.size()) {
            res += "\n\r";
        }
        res += status[result];
        res += "\n\r";
    }
    response.clear();
    for (int i = 0; i < res.size(); i++) {
        response.push_back(res[i]);
    }
}

bool BoardTestConsoleApp::parseTecCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    std::vector<std::string> tecStatus;
    tecStatus.push_back("OKAY");
    tecStatus.push_back("ERROR_ISENSE");
    tecStatus.push_back("ERROR_VSENSE");
    tecStatus.push_back("ERROR_REF_CURRENT_OUT_OF_RANGE");
    tecStatus.push_back("ERROR_SET_REF_CURRENT");
    tecStatus.push_back("ERROR_WAVE_FORM_OUT_OF_RANGE");
    tecStatus.push_back("ERROR_WAVEFORM_PERIOD_OUT_OF_RANGE");
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "enable") {
            result = regWrite(BoardTest::TEC_CONTROL, BoardTestTec::ENABLE);
            isParsingError = false;
        }
        else if (tokens[ACTION] == "disable") {
            result = regWrite(BoardTest::TEC_CONTROL, BoardTestTec::DISABLE);
            isParsingError = false;
        }
        else if (tokens[ACTION] == "reset") {
            result = regWrite(BoardTest::FAULT_RESET,
                                              BoardTestFault::FAULT_RESET_MASK);
            isParsingError = false;
        }
        else if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "isense" || tokens[ARGUMENT] == "vsense") {
                uint32 value;
                uint32 reg = (tokens[ARGUMENT] == "isense"
                           ?  BoardTest::TEC_ISENSE_VALUE
                           :  BoardTest::TEC_VSENSE_VALUE);
                result = regRead(reg, value);
                if (result == BoardTest::OKAY) {
                    uint32 status;
                    result = regRead(BoardTest::TEC_STATUS, status);
                    if (status != LibTec::OKAY) {
                        res = "tec status: " + tecStatus[status];
                    }
                    else {
                        float f = *reinterpret_cast<float*>(&value);
                        char t[16];
                        sprintf(t, "%.2f%s", f,
                                      tokens[ARGUMENT] == "isense" ? "A" : "V");
                        res = t;
                    }
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "iref") {
                uint32 value;
                result = regRead(BoardTest::TEC_IREF_VALUE, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2fA", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "waveformtype") {
                uint32 value;
                result = regRead(BoardTest::TEC_WAVEFORM_TYPE, value);
                if (result == BoardTest::OKAY) {
                    std::vector<std::string> waveformTypes;
                    waveformTypes.push_back("const");
                    waveformTypes.push_back("sin");
                    waveformTypes.push_back("tr");
                    waveformTypes.push_back("sq");
                    res = waveformTypes[value];
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "waveformperiod") {
                uint32 value;
                result = regRead(BoardTest::TEC_WAVEFORM_PERIOD, value);
                if (result == BoardTest::OKAY) {
                    char t[16];
                    sprintf(t, "%dms", value);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "waveform") {
                uint32 value;
                result = regRead(BoardTest::TEC_CONTROL, value);
                if (result == BoardTest::OKAY) {
                    res = value & BoardTestTec::START_WAVEFORM ? "running"  :
                          value & BoardTestTec::STOP_WAVEFORM  ? "stopped"  :
                                                                 "undefined";
                }
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "set" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "iref" && tokens.size() > VALUE) {
                float refCurr;
                if (sscanf(tokens[VALUE].c_str(), "%f", &refCurr) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&refCurr);
                   result = regWrite(BoardTest::TEC_IREF_VALUE, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::TEC_STATUS, value);
                       if (value != LibTec::OKAY) {
                           res = "tec  status: " + tecStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "waveformtype" && tokens.size() > VALUE) {
                std::map<std::string, int> waveformTypes;
                waveformTypes["const"] = LibTec::WAVEFORM_TYPE_CONSTANT;
                waveformTypes["sin"]   = LibTec::WAVEFORM_TYPE_SINUSOIDAL;
                waveformTypes["tr"]    = LibTec::WAVEFORM_TYPE_TRIANGULAR;
                waveformTypes["sq"]    = LibTec::WAVEFORM_TYPE_SQUARE;
                if (waveformTypes.find(tokens[VALUE]) != waveformTypes.end()) {
                    uint32 waveformType = waveformTypes[tokens[VALUE]];
                    result = regWrite(BoardTest::TEC_WAVEFORM_TYPE, waveformType);
                    if (result == BoardTest::OKAY) {
                        uint32 status;
                        result = regRead(BoardTest::TEC_STATUS, status);
                        if (status != LibTec::OKAY) {
                            res = "tec  status: " + tecStatus[status];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "waveformperiod" && tokens.size() > VALUE) {
                uint32 waveformPeriod;
                if (sscanf(tokens[VALUE].c_str(), "%d", &waveformPeriod) == 1) {
                    result = regWrite(BoardTest::TEC_WAVEFORM_PERIOD, waveformPeriod);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::TEC_STATUS, v);
                        if (v != LibTec::OKAY) {
                            res = "tec status: " + tecStatus[v];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "waveform" && tokens.size() > VALUE) {
                if (tokens[VALUE] == "start") {
                    result = regWrite(BoardTest::TEC_CONTROL,
                                                  BoardTestTec::START_WAVEFORM);
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "stop") {
                    result = regWrite(BoardTest::TEC_CONTROL,
                                                   BoardTestTec::STOP_WAVEFORM);
                    isParsingError = false;
                }
            }
        }
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseFaultCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "reset") {
            result = regWrite(BoardTest::FAULT_RESET,
                                              BoardTestFault::FAULT_RESET_MASK);
            isParsingError = false;
        }
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseThermistorCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "a") {
                uint32 temp;
                result = regRead(BoardTest::THERMISTOR_RESULT_AIN_A, temp);
                float f = *reinterpret_cast<float*>(&temp);
                char t[16];
                sprintf(t, "%.2fC", f);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "b") {
                uint32 temp;
                result = regRead(BoardTest::THERMISTOR_RESULT_AIN_B, temp);
                float f = *reinterpret_cast<float*>(&temp);
                char t[16];
                sprintf(t, "%.2fC", f);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "c") {
                uint32 temp;
                result = regRead(BoardTest::THERMISTOR_RESULT_AIN_C, temp);
                float f = *reinterpret_cast<float*>(&temp);
                char t[16];
                sprintf(t, "%.2fC", f);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "d") {
                uint32 temp;
                result = regRead(BoardTest::THERMISTOR_RESULT_AIN_D, temp);
                float f = *reinterpret_cast<float*>(&temp);
                char t[16];
                sprintf(t, "%.2fC", f);
                res = t;
                isParsingError = false;
            }
        }
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseLedCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "set" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "red" && tokens.size() > VALUE) {
                if (tokens[VALUE] == "on") {
                    result = regWrite(BoardTest::LED_CONTROL, BoardTestLed::RED_ON);
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "off") {
                    result = regWrite(BoardTest::LED_CONTROL, BoardTestLed::RED_OFF);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "green" && tokens.size() > VALUE) {
                if (tokens[VALUE] == "on") {
                    result = regWrite(BoardTest::LED_CONTROL, BoardTestLed::GREEN_ON);
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "off") {
                    result = regWrite(BoardTest::LED_CONTROL, BoardTestLed::GREEN_OFF);
                    isParsingError = false;
                }
            }
        }
        else if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            uint32 ledState;
            if (tokens[ARGUMENT] == "red") {
                result = regRead(BoardTest::LED_CONTROL, ledState);
                res = ledState & BoardTestLed::RED_ON  ? "on"       :
                      ledState & BoardTestLed::RED_OFF ? "off"      :
                                                         "undefined";
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "green") {
                result = regRead(BoardTest::LED_CONTROL, ledState);
                res = ledState & BoardTestLed::GREEN_ON  ? "on"       :
                      ledState & BoardTestLed::GREEN_OFF ? "off"      :
                                                           "undefined";
                isParsingError = false;
            }
        }
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseDacCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    std::vector<std::string> dacStatus;
    dacStatus.push_back("OKAY");
    dacStatus.push_back("ERROR_SET_VALUE_OUT_OF_RANGE");
    dacStatus.push_back("ERROR_TIME_OUT");
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "set" && tokens.size() > ARGUMENT) {
            float dacValue;
            if (sscanf(tokens[ARGUMENT].c_str(), "%f", &dacValue) == 1) {
                uint32 v = *reinterpret_cast<uint32*>(&dacValue);
                result = regWrite(BoardTest::DAC_VALUE, v);
                if (result == BoardTest::OKAY) {
                    result = regRead(BoardTest::DAC_STATUS, v);
                    if (v != LibDac::OKAY) {
                        res = "dac status: " + dacStatus[v];
                    }
                }
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "get") {
            uint32 dacValue;
            result = regRead(BoardTest::DAC_VALUE, dacValue);
            float f = *reinterpret_cast<float*>(&dacValue);
            char t[16];
            sprintf(t, "%.2fV", f);
            res = t;
            isParsingError = false;
        }
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseAdcCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    std::vector<std::string> adcStatus;
    adcStatus.push_back("OKAY");
    adcStatus.push_back("ERROR_INVALID_CHANNEL");
    adcStatus.push_back("ERROR_TIME_OUT");
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            int adcChannel;
            if (sscanf(tokens[ARGUMENT].c_str(), "%d", &adcChannel) == 1
            && (adcChannel & BoardTestAdc::ADC_CHANNEL_MASK) == adcChannel) {
                uint32 v = (adcChannel & BoardTestAdc::ADC_CHANNEL_MASK)
                                  << BoardTestAdc::ADC_CHANNEL_SHIFT
                         |  BoardTestAdc::ADC_START_MASK
                                  << BoardTestAdc::ADC_START_MASK_SHIFT;
                result = regWrite(BoardTest::ADC_CONTROL, v);
                if (result == BoardTest::OKAY) {
                    result = regRead(BoardTest::ADC_STATUS, v);
                    if (v != LibAdc::OKAY) {
                        res = "adc status: " + adcStatus[v];
                    }
                    else {
                        result = regRead(BoardTest::ADC_RESULT, v);
                        float f = *reinterpret_cast<float*>(&v);
                        char t[16];
                        sprintf(t, "%.2fV", f);
                        res = t;
                    }
                }
                isParsingError = false;
            }
        }
    }
    return isParsingError;
}
