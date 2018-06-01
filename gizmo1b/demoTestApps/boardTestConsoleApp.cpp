#include <string>
#include <vector>
#include <stdio.h>
#include <map>
#include "libSci2.h"
#include "boardTestDac.h"
#include "boardTestTec.h"
#include "boardTestFault.h"
#include "boardTestLed.h"
#include "boardTestAdc.h"
#include "boardTestMotor.h"
#include "boardTestFan.h"
#include "boardTestDio.h"
#include "boardTestThermistor.h"
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
    int rxQueueLength = 2048;
    int txQueueLength = 2048;
    LibSci2 libSci(rxQueueLength, txQueueLength);
    bool resetSci = true;
    std::vector<uint8> message;
    message.reserve(rxQueueLength);
    std::vector<uint8> response;
    response.reserve(txQueueLength);
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
    help += "dac set [0,5](V)\n\r";
    help += "dac get\n\r";
    help += "adc get 0..5|all\n\r";
    help += "led set green|red on|off\n\r";
    help += "led get green|red\n\r";
    help += "fault reset\n\r";
    help += "fault get ntc|alarms\n\r";
    help += "tec enable|disable\n\r";
    help += "tec get enable\n\r";
    help += "tec get isense|vsense|iref|waveformtype|waveformperiod|waveform|closedloop\n\r";
    help += "tec set iref [-15,15](A)\n\r";
    help += "tec set waveformtype sin|tr|sq|const|custom\n\r";
    help += "tec set waveformperiod 2..10,000(ms)\n\r";
    help += "tec set waveform start|stop\n\r";
    help += "tec set closedloop enable|disable\n\r";
    help += "tec set propgain [0.01,100]\n\r";
    help += "tec get propgain\n\r";
    help += "tec set intgain [0,100]\n\r";
    help += "tec get intgain\n\r";
    help += "tec set dergain [0,100]\n\r";
    help += "tec get dergain\n\r";
    help += "tec set customindex reset\n\r";
    help += "tec set customindex inc\n\r";
    help += "tec set customtime 0..9,999(ms)\n\r";
    help += "tec set customiref [-15,15](A)\n\r";
    help += "tec set customcycles 0..4,294,967,296\n\r";
    help += "tec get customindex|customtime|customiref|customcycles\n\r";
    help += "heater enable|disable\n\r";
    help += "heater get enable\n\r";
    help += "heater get tref|imax|closedloop|tin\n\r";
    help += "heater set tref [0,100](deg C)\n\r";
    help += "heater set imax [0,15](A)\n\r";
    help += "heater set tin 1..4\n\r";
    help += "heater set closedloop enable|disable\n\r";
    help += "heater set propgain [0.01,100]\n\r";
    help += "heater get propgain\n\r";
    help += "heater set intgain [0,100]\n\r";
    help += "heater get intgain\n\r";
    help += "heater set dergain [0,100]\n\r";
    help += "heater get dergain\n\r";
    help += "heater set customindex reset\n\r";
    help += "heater set customindex inc\n\r";
    help += "heater set customtime 0..10,000(s)\n\r";
    help += "heater set customtref [0,100](deg C)\n\r";
    help += "heater set customcycles 0..4,294,967,296\n\r";
    help += "heater get customindex|customtime|customtref|customcycles|waveform\n\r";
    help += "heater set waveform start|stop\n\r";
    help += "thermistor get a|b|c|d|all\n\r";
    help += "thermistor set type USP12387|SC30F103AN\n\r";
    help += "thermistor get type\n\r";
    help += "motor reset|initialize|limp|energize|stop\n\r";
    help += "motor get regaddress|regvalue|step|abspos|relpos|pos|status\n\r";
    help += "motor set regaddress|regvalue 0x<hex>\n\r";
    help += "motor set step full|half|1/4|1/8|1/16|1/32|1/64|1/128\n\r";
    help += "motor regread|regwrite\n\r";
    help += "motor set abspos -2097152..2097151\n\r";
    help += "motor set relpos 0..4194303\n\r";
    help += "motor moveabs\n\r";
    help += "motor moverel forward|reverse\n\r";
    help += "motor cycle\n\r";
    help += "fan set duty1|duty2 0..100\n\r";
    help += "fan set per1|per2 [10.0,1000000.0](us)\n\r";
    help += "fan get duty1|duty2|per1|per2|sens1|sens2\n\r";
    help += "dio get 0..9|all\n\r";
    help += "dio set|clear 0..7\n\r";
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
    if (msg.size() == 0) {
        msg = m_prevMsg;
    }
    if (msg.size()) {
        m_prevMsg = msg;
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
            else if (tokens[COMPONENT] == "heater") {
                isParsingError = parseHeaterCommand(tokens, res, result);
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
            else if (tokens[COMPONENT] == "motor") {
                isParsingError = parseMotorCommand(tokens, res, result);
            }
            else if (tokens[COMPONENT] == "fan") {
                isParsingError = parseFanCommand(tokens, res, result);
            }
            else if (tokens[COMPONENT] == "dio") {
                isParsingError = parseDioCommand(tokens, res, result);
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
    if (res.size() > response.capacity()) {
        int size = res.size();
        char error[256];
        sprintf(error, "Error: resp message size: %d exceeds buffer capacity: %d\n\r",
                                                     size, response.capacity());
        res = error;
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
    tecStatus.push_back("ERROR_CUSTOM_WAVEFORM_EMPTY");
    tecStatus.push_back("ERROR_PROPORTIONAL_GAIN_OUT_OF_RANGE");
    tecStatus.push_back("ERROR_INTEGRAL_GAIN_OUT_OF_RANGE");
    tecStatus.push_back("ERROR_DERIVATIVE_GAIN_OUT_OF_RANGE");
    tecStatus.push_back("ERROR_CUSTOM_WAVEFORM_TIME_NOT_RISING");
    tecStatus.push_back("ERROR_CUSTOM_WAVEFORM_NON_ZERO_START_TIME");
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "enable") {
            result = regWrite(BoardTest::TEC_CONTROL, BoardTestTec::ENABLE);
            isParsingError = false;
        }
        else if (tokens[ACTION] == "disable") {
            result = regWrite(BoardTest::TEC_CONTROL, BoardTestTec::DISABLE);
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
            else if (tokens[ARGUMENT] == "enable") {
                uint32 value;
                result = regRead(BoardTest::TEC_CONTROL, value);
                if (result == BoardTest::OKAY) {
                    res = value & BoardTestTec::DISABLE ? "disabled" :
                          value & BoardTestTec::ENABLE  ? "enabled"  :
                                                          "undefined";
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
                    waveformTypes.push_back("custom");
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
            else if (tokens[ARGUMENT] == "propgain") {
                uint32 value;
                result = regRead(BoardTest::TEC_PROPORTIONAL_GAIN, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2f", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "intgain") {
                uint32 value;
                result = regRead(BoardTest::TEC_INTEGRAL_GAIN, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2f", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "dergain") {
                uint32 value;
                result = regRead(BoardTest::TEC_DERIVATIVE_GAIN, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2f", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "closedloop") {
                uint32 value;
                result = regRead(BoardTest::TEC_CONTROL, value);
                if (result == BoardTest::OKAY) {
                    res = value & BoardTestTec::CLOSED_LOOP_DISABLE ? "disabled" :
                          value & BoardTestTec::CLOSED_LOOP_ENABLE  ? "enabled"  :
                                                                      "undefined";
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "customindex") {
                uint32 value;
                result = regRead(BoardTest::TEC_WAVEFORM_SAMPLE_INDEX, value);
                if (result == BoardTest::OKAY) {
                    char t[16];
                    sprintf(t, "%d", value);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "customtime") {
                uint32 value;
                result = regRead(BoardTest::TEC_WAVEFORM_SAMPLE_TIME, value);
                if (result == BoardTest::OKAY) {
                    char t[16];
                    sprintf(t, "%dms", value);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "customiref") {
                uint32 value;
                result = regRead(BoardTest::TEC_WAVEFORM_SAMPLE_IREF, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2fA", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "customcycles") {
                uint32 value;
                result = regRead(BoardTest::TEC_WAVEFORM_CYCLES, value);
                if (result == BoardTest::OKAY) {
                    char t[16];
                    sprintf(t, "%d", value);
                    res = t;
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
                waveformTypes["const"]  = LibTec::WAVEFORM_TYPE_CONSTANT;
                waveformTypes["sin"]    = LibTec::WAVEFORM_TYPE_SINUSOIDAL;
                waveformTypes["tr"]     = LibTec::WAVEFORM_TYPE_TRIANGULAR;
                waveformTypes["sq"]     = LibTec::WAVEFORM_TYPE_SQUARE;
                waveformTypes["custom"] = LibTec::WAVEFORM_TYPE_CUSTOM;
                if (waveformTypes.find(tokens[VALUE]) != waveformTypes.end()) {
                    uint32 waveformType = waveformTypes[tokens[VALUE]];
                    result = regWrite(BoardTest::TEC_WAVEFORM_TYPE,
                                                                  waveformType);
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
                    result = regWrite(BoardTest::TEC_WAVEFORM_PERIOD,
                                                                waveformPeriod);
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
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::TEC_STATUS, v);
                        if (v != LibTec::OKAY) {
                            res = "tec status: " + tecStatus[v];
                        }
                    }
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "stop") {
                    result = regWrite(BoardTest::TEC_CONTROL,
                                                   BoardTestTec::STOP_WAVEFORM);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "closedloop" && tokens.size() > VALUE) {
                if (tokens[VALUE] == "enable") {
                    result = regWrite(BoardTest::TEC_CONTROL,
                                              BoardTestTec::CLOSED_LOOP_ENABLE);
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "disable") {
                    result = regWrite(BoardTest::TEC_CONTROL,
                                             BoardTestTec::CLOSED_LOOP_DISABLE);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "propgain" && tokens.size() > VALUE) {
                float gain;
                if (sscanf(tokens[VALUE].c_str(), "%f", &gain) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&gain);
                   result = regWrite(BoardTest::TEC_PROPORTIONAL_GAIN, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::TEC_STATUS, value);
                       if (value != LibTec::OKAY) {
                           res = "tec  status: " + tecStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "intgain" && tokens.size() > VALUE) {
                float gain;
                if (sscanf(tokens[VALUE].c_str(), "%f", &gain) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&gain);
                   result = regWrite(BoardTest::TEC_INTEGRAL_GAIN, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::TEC_STATUS, value);
                       if (value != LibTec::OKAY) {
                           res = "tec  status: " + tecStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "dergain" && tokens.size() > VALUE) {
                float gain;
                if (sscanf(tokens[VALUE].c_str(), "%f", &gain) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&gain);
                   result = regWrite(BoardTest::TEC_DERIVATIVE_GAIN, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::TEC_STATUS, value);
                       if (value != LibTec::OKAY) {
                           res = "tec  status: " + tecStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "customindex" && tokens.size() > VALUE) {
                if (tokens[VALUE] == "reset") {
                    result = regWrite(BoardTest::TEC_CONTROL,
                                     BoardTestTec::CUSTOM_WAVEFORM_RESET_INDEX);
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "inc") {
                    result = regWrite(BoardTest::TEC_CONTROL,
                                       BoardTestTec::CUSTOM_WAVEFORM_INC_INDEX);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "customtime" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "%d", &value) == 1) {
                    result = regWrite(BoardTest::TEC_WAVEFORM_SAMPLE_TIME, value);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "customiref" && tokens.size() > VALUE) {
                float customiref;
                if (sscanf(tokens[VALUE].c_str(), "%f", &customiref) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&customiref);
                   result = regWrite(BoardTest::TEC_WAVEFORM_SAMPLE_IREF, value);
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "customcycles" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "%d", &value) == 1) {
                    result = regWrite(BoardTest::TEC_WAVEFORM_CYCLES, value);
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
        else if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "ntc") {
                uint32 value;
                result = regRead(BoardTest::FAULT_NTC_PRESENT, value);
                res.clear();
                std::map<int, std::string> ntcFlags;
                ntcFlags[BoardTestFault::NTC1_PRESENT] = "NTC1_PRESENT";
                ntcFlags[BoardTestFault::NTC2_PRESENT] = "NTC2_PRESENT";
                for (std::map<int, std::string>::iterator it = ntcFlags.begin();
                                                   it != ntcFlags.end(); it++) {
                    if (value & it->first) {
                        res += it->second + "|";
                    }
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "alarms") {
                uint32 value;
                result = regRead(BoardTest::FAULT_STATE, value);
                res.clear();
                std::map<int, std::string> faultFlags;
                faultFlags[BoardTestFault::TEC_OCD_POS] = "TEC_OCD_POS";
                faultFlags[BoardTestFault::TEC_OCD_NEG] = "TEC_OCD_NEG";
                faultFlags[BoardTestFault::OVERTEMP1]   = "OVERTEMP1";
                faultFlags[BoardTestFault::OVERTEMP2]   = "OVERTEMP2";
                for (std::map<int, std::string>::iterator it = faultFlags.begin();
                                                 it != faultFlags.end(); it++) {
                    if (value & it->first) {
                        res += it->second + "|";
                    }
                }
                isParsingError = false;
            }
        }
    }
    return isParsingError;
}
//help += "thermistor set type USP12387|SC30F103AN\n\r";
//help += "thermistor get type\n\r";
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
            else if (tokens[ARGUMENT] == "all") {
                std::map<int, std::string> thermNames;
                thermNames[BoardTest::THERMISTOR_RESULT_AIN_A] = "[a]:";
                thermNames[BoardTest::THERMISTOR_RESULT_AIN_B] = "[b]:";
                thermNames[BoardTest::THERMISTOR_RESULT_AIN_C] = "[c]:";
                thermNames[BoardTest::THERMISTOR_RESULT_AIN_D] = "[d]:";
                for (std::map<int, std::string>::iterator it = thermNames.begin();
                                                 it != thermNames.end(); it++) {
                    uint32 temp;
                    result = regRead(it->first, temp);
                    float f = *reinterpret_cast<float*>(&temp);
                    char t[16];
                    sprintf(t, "%s%.2fC ", it->second.c_str(), f);
                    res += t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "type") {
                std::map<int, std::string> thermTypes;
                thermTypes[BoardTestThermistor::SC30F103AN] = "SC30F103AN";
                thermTypes[BoardTestThermistor::USP12387]   = "USP12387";
                uint32 type;
                result = regRead(BoardTest::THERMISTOR_TYPE, type);
                char t[16];
                sprintf(t, "%s", thermTypes.find(type) != thermTypes.end()
                                                      ? thermTypes[type].c_str()
                                                      : "unknown");
                res = t;
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "set" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "type" && tokens.size() > VALUE) {
                std::map<std::string, int> thermTypes;
                thermTypes["SC30F103AN"] = BoardTestThermistor::SC30F103AN;
                thermTypes["USP12387"]   = BoardTestThermistor::USP12387;
                if (thermTypes.find(tokens[VALUE]) != thermTypes.end()) {
                    result = regWrite(BoardTest::THERMISTOR_TYPE,
                                                     thermTypes[tokens[VALUE]]);
                    isParsingError = false;
                }
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
            if (tokens[ARGUMENT] == "all") {
                for (int i = 0; i < 5; i++) {
                    uint32 v = (i & BoardTestAdc::ADC_CHANNEL_MASK)
                                      << BoardTestAdc::ADC_CHANNEL_SHIFT
                             |  BoardTestAdc::ADC_START_MASK
                                      << BoardTestAdc::ADC_START_MASK_SHIFT;
                    result = regWrite(BoardTest::ADC_CONTROL, v);
                    if (result == BoardTest::OKAY) {
                        result = regRead(BoardTest::ADC_STATUS, v);
                        if (v != LibAdc::OKAY) {
                            res = "adc status: " + adcStatus[v];
                            break;
                        }
                        else {
                            result = regRead(BoardTest::ADC_RESULT, v);
                            float f = *reinterpret_cast<float*>(&v);
                            char t[16];
                            sprintf(t, "[%d]:%.2fV ", i, f);
                            res += t;
                        }
                    }
                }
                isParsingError = false;
            }
            else {
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
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseMotorCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    std::vector<std::string> commStatus;
    commStatus.push_back("OKAY");
    commStatus.push_back("ERROR_INVALID_DIRECTION");
    commStatus.push_back("ERROR_STEPS_COUNT");
    commStatus.push_back("ERROR_INVALID_MICROSTEPS");
    commStatus.push_back("ERROR_TIME_OUT");
    commStatus.push_back("ERROR_INVALID_REG");
    std::map<int, std::string> statusFlags;
    statusFlags[LibMotor::HIZ]                           = "HIZ";
    statusFlags[LibMotor::BUSY]                          = "BUSY";
    statusFlags[LibMotor::SW_CLOSED]                     = "SW_CLOSED";
    statusFlags[LibMotor::SW_OPEN]                       = "SW_OPEN";
    statusFlags[LibMotor::DIR_FORWARD]                   = "DIR_FORWARD";
    statusFlags[LibMotor::DIR_REVERSE]                   = "DIR_REVERSE";
    statusFlags[LibMotor::STOPPED]                       = "STOPPED";
    statusFlags[LibMotor::ACCELERATING]                  = "ACCELERATING";
    statusFlags[LibMotor::DECELERATING]                  = "DECELERATING";
    statusFlags[LibMotor::RUN_AT_CONST_SPEED]            = "RUN_AT_CONST_SPEED";
    statusFlags[LibMotor::THERMAL_WARNING]               = "THERMAL_WARNING";
    statusFlags[LibMotor::THERMAL_SHUTDOWN]              = "THERMAL_SHUTDOWN";
    statusFlags[LibMotor::OVERCURRENT]                   = "OVERCURRENT";
    statusFlags[LibMotor::BRIDGE_A_STALL]                = "BRIDGE_A_STALL";
    statusFlags[LibMotor::BRIDGE_B_STALL]                = "BRIDGE_B_STALL";
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            uint32 value;
            if (tokens[ARGUMENT] == "regaddress") {
                result = regRead(BoardTest::MOTOR_REGISTER_ADDRESS, value);
                char t[16];
                sprintf(t, "0x%08x", value);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "regvalue") {
                result = regRead(BoardTest::MOTOR_REGISTER_VALUE, value);
                char t[16];
                sprintf(t, "0x%08x", value);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "step") {
                result = regRead(BoardTest::MOTOR_CONTROL, value);
                if (result == BoardTest::OKAY) {
                    uint32 v;
                    result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                    if (v != LibMotor::OKAY) {
                        res = "motor comm status: " + commStatus[v];
                    }
                    else {
                        std::map<int, std::string> step;
                        step[BoardTestMotor::STEP_FULL]          = "full";
                        step[BoardTestMotor::STEP_HALF]          = "half";
                        step[BoardTestMotor::STEP_MICRO_1_4TH]   = "1/4";
                        step[BoardTestMotor::STEP_MICRO_1_8TH]   = "1/8";
                        step[BoardTestMotor::STEP_MICRO_1_16TH]  = "1/16";
                        step[BoardTestMotor::STEP_MICRO_1_32TH]  = "1/32";
                        step[BoardTestMotor::STEP_MICRO_1_64TH]  = "1/64";
                        step[BoardTestMotor::STEP_MICRO_1_128TH] = "1/128";
                        char t[16];
                        sprintf(t, "%s", step[value].c_str());
                        res = t;
                    }
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "abspos") {
                result = regRead(BoardTest::MOTOR_ABSOLUTE_POSITION, value);
                char t[16];
                sint32 v = *reinterpret_cast<sint32*>(&value);
                sprintf(t, "%d", v);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "relpos") {
                result = regRead(BoardTest::MOTOR_RELATIVE_POSITION, value);
                char t[16];
                sprintf(t, "%d", value);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "pos") {
                result = regRead(BoardTest::MOTOR_CURRENT_POSITION, value);
                if (result == BoardTest::OKAY) {
                    uint32 v;
                    result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                    if (v != LibMotor::OKAY) {
                        res = "motor comm status: " + commStatus[v];
                    }
                    else {
                        char t[16];
                        sint32 position = *reinterpret_cast<sint32*>(&value);
                        sprintf(t, "%d", position);
                        res = t;
                    }
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "status") {
                result = regRead(BoardTest::MOTOR_STATUS, value);
                if (result == BoardTest::OKAY) {
                    uint32 v;
                    result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                    if (v != LibMotor::OKAY) {
                        res = "motor comm status: " + commStatus[v];
                    }
                    else {
                        res.clear();
                        for (std::map<int, std::string>::iterator it = statusFlags.begin();
                                                it != statusFlags.end(); it++) {
                            if (value & it->first) {
                                res += it->second + "|";
                            }
                        }
                    }
                }
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "set" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "regaddress" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "0x%x", &value) == 1) {
                    result = regWrite(BoardTest::MOTOR_REGISTER_ADDRESS, value);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                        if (v != LibMotor::OKAY) {
                            res = "motor comm status: " + commStatus[v];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "regvalue" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "0x%x", &value) == 1) {
                    result = regWrite(BoardTest::MOTOR_REGISTER_VALUE, value);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                        if (v != LibMotor::OKAY) {
                            res = "motor comm status: " + commStatus[v];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "step" && tokens.size() > VALUE) {
                std::map<std::string, int> step;
                step["full"]  = BoardTestMotor::STEP_FULL;
                step["half"]  = BoardTestMotor::STEP_HALF;
                step["1/4"]   = BoardTestMotor::STEP_MICRO_1_4TH;
                step["1/8"]   = BoardTestMotor::STEP_MICRO_1_8TH;
                step["1/16"]  = BoardTestMotor::STEP_MICRO_1_16TH;
                step["1/32"]  = BoardTestMotor::STEP_MICRO_1_32TH;
                step["1/64"]  = BoardTestMotor::STEP_MICRO_1_64TH;
                step["1/128"] = BoardTestMotor::STEP_MICRO_1_128TH;
                if (step.find(tokens[VALUE]) != step.end()) {
                    uint32 value = step[tokens[VALUE]];
                    result = regWrite(BoardTest::MOTOR_CONTROL, value);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                        if (v != LibMotor::OKAY) {
                            res = "motor comm status: " + commStatus[v];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "abspos" && tokens.size() > VALUE) {
                sint32 value;
                if (sscanf(tokens[VALUE].c_str(), "%d", &value) == 1) {
                    uint32 v = *reinterpret_cast<uint32*>(&value);
                    result = regWrite(BoardTest::MOTOR_ABSOLUTE_POSITION, v);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "relpos" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "%d", &value) == 1) {
                    result = regWrite(BoardTest::MOTOR_RELATIVE_POSITION, value);
                    isParsingError = false;
                }
            }
        }
        else if (tokens[ACTION] == "moveabs") {
            result = regWrite(BoardTest::MOTOR_CONTROL,
                                                 BoardTestMotor::MOVE_ABSOLUTE);
            if (result == BoardTest::OKAY) {
                uint32 v;
                result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                if (v != LibMotor::OKAY) {
                    res = "motor comm status: " + commStatus[v];
                }
            }
            isParsingError = false;
        }
        else if (tokens[ACTION] == "moverel" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "forward") {
                result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::DIRECTION_FORWARD
                                                          | BoardTestMotor::MOVE_RELATIVE);
                if (result == BoardTest::OKAY) {
                    uint32 v;
                    result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                    if (v != LibMotor::OKAY) {
                        res = "motor comm status: " + commStatus[v];
                    }
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "reverse") {
                result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::DIRECTION_REVERSE
                                                          | BoardTestMotor::MOVE_RELATIVE);
                if (result == BoardTest::OKAY) {
                    uint32 v;
                    result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                    if (v != LibMotor::OKAY) {
                        res = "motor comm status: " + commStatus[v];
                    }
                }
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "cycle") {
            result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::CYCLE);
            isParsingError = false;
        }
        else if (tokens[ACTION] == "reset") {
            result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::RESET);
            isParsingError = false;
        }
        else if (tokens[ACTION] == "initialize") {
            result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::INITIALIZE);
            if (result == BoardTest::OKAY) {
                uint32 v;
                result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                if (v != LibMotor::OKAY) {
                    res = "motor comm status: " + commStatus[v];
                }
            }
            isParsingError = false;
        }
        else if (tokens[ACTION] == "limp") {
            result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::LIMP);
            if (result == BoardTest::OKAY) {
                uint32 v;
                result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                if (v != LibMotor::OKAY) {
                    res = "motor comm status: " + commStatus[v];
                }
            }
            isParsingError = false;
        }
        else if (tokens[ACTION] == "energize") {
            result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::ENERGIZE);
            if (result == BoardTest::OKAY) {
                uint32 v;
                result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                if (v != LibMotor::OKAY) {
                    res = "motor comm status: " + commStatus[v];
                }
            }
            isParsingError = false;
        }
        else if (tokens[ACTION] == "stop") {
            result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::STOP);
            if (result == BoardTest::OKAY) {
                uint32 v;
                result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                if (v != LibMotor::OKAY) {
                    res = "motor comm status: " + commStatus[v];
                }
            }
            isParsingError = false;
        }
        else if (tokens[ACTION] == "regread") {
            result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::REGISTER_READ);
            if (result == BoardTest::OKAY) {
                uint32 v;
                result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                if (v != LibMotor::OKAY) {
                    res = "motor comm status: " + commStatus[v];
                }
            }
            isParsingError = false;
        }
        else if (tokens[ACTION] == "regwrite") {
            result = regWrite(BoardTest::MOTOR_CONTROL, BoardTestMotor::REGISTER_WRITE);
            if (result == BoardTest::OKAY) {
                uint32 v;
                result = regRead(BoardTest::MOTOR_COMMUNICATION_STATUS, v);
                if (v != LibMotor::OKAY) {
                    res = "motor comm status: " + commStatus[v];
                }
            }
            isParsingError = false;
        }
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseFanCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    std::vector<std::string> status;
    status.push_back("OKAY");
    status.push_back("ERROR_DUTY_CYCLE_OUT_OF_RANGE");
    status.push_back("ERROR_PERIOD_IN_US_OUT_OF_RANGE");
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "per1") {
                uint32 period;
                result = regRead(BoardTest::FAN_PWM1_PERIOD_IN_US, period);
                float f = *reinterpret_cast<float*>(&period);
                char t[16];
                sprintf(t, "%.2fus", f);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "duty1") {
                uint32 value;
                result = regRead(BoardTest::FAN_PWM1_DUTY_CYCLE, value);
                char t[16];
                sprintf(t, "%d%%", value);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "per2") {
                uint32 period;
                result = regRead(BoardTest::FAN_PWM2_PERIOD_IN_US, period);
                float f = *reinterpret_cast<float*>(&period);
                char t[16];
                sprintf(t, "%.2fus", f);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "duty2") {
                uint32 value;
                result = regRead(BoardTest::FAN_PWM2_DUTY_CYCLE, value);
                char t[16];
                sprintf(t, "%d%%", value);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "sens1") {
                uint32 rpm;
                result = regRead(BoardTest::FAN_SENSOR1_RPM, rpm);
                float f = *reinterpret_cast<float*>(&rpm);
                char t[16];
                sprintf(t, "%.2fRPM", f);
                res = t;
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "sens2") {
                uint32 rpm;
                result = regRead(BoardTest::FAN_SENSOR2_RPM, rpm);
                float f = *reinterpret_cast<float*>(&rpm);
                char t[16];
                sprintf(t, "%.2fRPM.", f);
                res = t;
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "set" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "duty1" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "%d", &value) == 1) {
                    result = regWrite(BoardTest::FAN_PWM1_DUTY_CYCLE, value);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::FAN_STATUS, v);
                        if (v != LibWrapHet::OKAY) {
                            res = "fan status: " + status[v];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "duty2" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "%d", &value) == 1) {
                    result = regWrite(BoardTest::FAN_PWM2_DUTY_CYCLE, value);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::FAN_STATUS, v);
                        if (v != LibWrapHet::OKAY) {
                            res = "fan status: " + status[v];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "per1" && tokens.size() > VALUE) {
                float period;
                if (sscanf(tokens[VALUE].c_str(), "%f", &period) == 1) {
                    uint32 value = *reinterpret_cast<uint32*>(&period);
                    result = regWrite(BoardTest::FAN_PWM1_PERIOD_IN_US, value);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::FAN_STATUS, v);
                        if (v != LibWrapHet::OKAY) {
                            res = "fan status: " + status[v];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "per2" && tokens.size() > VALUE) {
                float period;
                if (sscanf(tokens[VALUE].c_str(), "%f", &period) == 1) {
                    uint32 value = *reinterpret_cast<uint32*>(&period);
                    result = regWrite(BoardTest::FAN_PWM2_PERIOD_IN_US, value);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::FAN_STATUS, v);
                        if (v != LibWrapHet::OKAY) {
                            res = "fan status: " + status[v];
                        }
                    }
                    isParsingError = false;
                }
            }
        }
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseDioCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            uint32 pin;
            std::map<int, int> pinsMap;
            pinsMap[0] = BoardTestDio::DIN_0_STATE;
            pinsMap[1] = BoardTestDio::DIN_1_STATE;
            pinsMap[2] = BoardTestDio::DIN_2_STATE;
            pinsMap[3] = BoardTestDio::DIN_3_STATE;
            pinsMap[4] = BoardTestDio::DIN_4_STATE;
            pinsMap[5] = BoardTestDio::DIN_5_STATE;
            pinsMap[6] = BoardTestDio::DIN_6_STATE;
            pinsMap[7] = BoardTestDio::DIN_7_STATE;
            pinsMap[8] = BoardTestDio::DIN_8_STATE;
            pinsMap[9] = BoardTestDio::DIN_9_STATE;
            if (tokens[ARGUMENT] == "all") {
                uint32 value;
                result = regRead(BoardTest::DIO_IN, value);
                for (std::map<int, int>::iterator it = pinsMap.begin();
                                                    it != pinsMap.end(); it++) {
                    char t[16];
                    sprintf(t, "[%d]:%d ", it->first,
                                            value & pinsMap[it->first] ? 1 : 0);
                    res += t;
                }
                isParsingError = false;
            }
            else if (sscanf(tokens[ARGUMENT].c_str(), "%d", &pin) == 1
             && pinsMap.find(pin) != pinsMap.end()) {
                uint32 value;
                result = regRead(BoardTest::DIO_IN, value);
                res = value & pinsMap[pin] ? "set" : "clear";
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "set" && tokens.size() > ARGUMENT) {
            uint32 pin;
            std::map<int, int> pinsMap;
            pinsMap[0] = BoardTestDio::DOUT_0_ON;
            pinsMap[1] = BoardTestDio::DOUT_1_ON;
            pinsMap[2] = BoardTestDio::DOUT_2_ON;
            pinsMap[3] = BoardTestDio::DOUT_3_ON;
            pinsMap[4] = BoardTestDio::DOUT_4_ON;
            pinsMap[5] = BoardTestDio::DOUT_5_ON;
            pinsMap[6] = BoardTestDio::DOUT_6_ON;
            pinsMap[7] = BoardTestDio::DOUT_7_ON;
            if (sscanf(tokens[ARGUMENT].c_str(), "%d", &pin) == 1
             && pinsMap.find(pin) != pinsMap.end()) {
                result = regWrite(BoardTest::DIO_OUT, pinsMap[pin]);
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "clear" && tokens.size() > ARGUMENT) {
            uint32 pin;
            std::map<int, int> pinsMap;
            pinsMap[0] = BoardTestDio::DOUT_0_OFF;
            pinsMap[1] = BoardTestDio::DOUT_1_OFF;
            pinsMap[2] = BoardTestDio::DOUT_2_OFF;
            pinsMap[3] = BoardTestDio::DOUT_3_OFF;
            pinsMap[4] = BoardTestDio::DOUT_4_OFF;
            pinsMap[5] = BoardTestDio::DOUT_5_OFF;
            pinsMap[6] = BoardTestDio::DOUT_6_OFF;
            pinsMap[7] = BoardTestDio::DOUT_7_OFF;
            if (sscanf(tokens[ARGUMENT].c_str(), "%d", &pin) == 1
             && pinsMap.find(pin) != pinsMap.end()) {
                result = regWrite(BoardTest::DIO_OUT, pinsMap[pin]);
                isParsingError = false;
            }
        }
    }
    return isParsingError;
}

bool BoardTestConsoleApp::parseHeaterCommand(std::vector<std::string>& tokens,
                                                  std::string& res, int& result)
{
    bool isParsingError = true;
    std::vector<std::string> heaterStatus;
    heaterStatus.push_back("HEATER_OKAY");
    heaterStatus.push_back("ERROR_HEATER_TREF_OUT_OF_RANGE");
    heaterStatus.push_back("ERROR_HEATER_IMAX_OUT_OF_RANGE");
    heaterStatus.push_back("ERROR_HEATER_PROPORTIONAL_GAIN_OUT_OF_RANGE");
    heaterStatus.push_back("ERROR_HEATER_INTEGRAL_GAIN_OUT_OF_RANGE");
    heaterStatus.push_back("ERROR_HEATER_DERIVATIVE_GAIN_OUT_OF_RANGE");
    heaterStatus.push_back("ERROR_HEATER_TIN_SELECT_OUT_OF_RANGE");
    heaterStatus.push_back("ERROR_HEATER_TIN");
    heaterStatus.push_back("ERROR_HEATER_CUSTOM_WAVEFORM_EMPTY");
    heaterStatus.push_back("ERROR_HEATER_CUSTOM_WAVEFORM_TIME_NOT_RISING");
    heaterStatus.push_back("ERROR_HEATER_CUSTOM_WAVEFORM_NON_ZERO_START_TIME");
    if (tokens.size() > ACTION) {
        if (tokens[ACTION] == "enable") {
            result = regWrite(BoardTest::HEATER_CONTROL, BoardTestTec::HEATER_ENABLE);
            isParsingError = false;
        }
        else if (tokens[ACTION] == "disable") {
            result = regWrite(BoardTest::HEATER_CONTROL, BoardTestTec::HEATER_DISABLE);
            isParsingError = false;
        }
        else if (tokens[ACTION] == "get" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "enable") {
                uint32 value;
                result = regRead(BoardTest::HEATER_CONTROL, value);
                if (result == BoardTest::OKAY) {
                    res = value & BoardTestTec::HEATER_DISABLE ? "disabled" :
                          value & BoardTestTec::HEATER_ENABLE  ? "enabled"  :
                                                                 "undefined";
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "tref") {
                uint32 value;
                result = regRead(BoardTest::HEATER_TREF_VALUE, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2fdegC", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "imax") {
                uint32 value;
                result = regRead(BoardTest::HEATER_IMAX_VALUE, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2fA", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "tin") {
                uint32 value;
                result = regRead(BoardTest::HEATER_TIN_SELECT, value);
                if (result == BoardTest::OKAY) {
                    std::map<int, std::string> tin;
                    tin[LibTec::HEATER_T1_SELECT] = "T1";
                    tin[LibTec::HEATER_T2_SELECT] = "T2";
                    tin[LibTec::HEATER_T3_SELECT] = "T3";
                    tin[LibTec::HEATER_T4_SELECT] = "T4";
                    char t[16];
                    sprintf(t, "%s", tin[value].c_str());
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "propgain") {
                uint32 value;
                result = regRead(BoardTest::HEATER_PROPORTIONAL_GAIN, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2f", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "intgain") {
                uint32 value;
                result = regRead(BoardTest::HEATER_INTEGRAL_GAIN, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2f", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "dergain") {
                uint32 value;
                result = regRead(BoardTest::HEATER_DERIVATIVE_GAIN, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2f", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "closedloop") {
                uint32 value;
                result = regRead(BoardTest::HEATER_CONTROL, value);
                if (result == BoardTest::OKAY) {
                    res = value & BoardTestTec::HEATER_CLOSED_LOOP_DISABLE ? "disabled" :
                          value & BoardTestTec::HEATER_CLOSED_LOOP_ENABLE  ? "enabled"  :
                                                                             "undefined";
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "customindex") {
                uint32 value;
                result = regRead(BoardTest::HEATER_WAVEFORM_SAMPLE_INDEX, value);
                if (result == BoardTest::OKAY) {
                    char t[16];
                    sprintf(t, "%d", value);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "customtime") {
                uint32 value;
                result = regRead(BoardTest::HEATER_WAVEFORM_SAMPLE_TIME, value);
                if (result == BoardTest::OKAY) {
                    char t[16];
                    sprintf(t, "%ds", value);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "customtref") {
                uint32 value;
                result = regRead(BoardTest::HEATER_WAVEFORM_SAMPLE_TREF, value);
                if (result == BoardTest::OKAY) {
                    float f = *reinterpret_cast<float*>(&value);
                    char t[16];
                    sprintf(t, "%.2fdegC", f);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "customcycles") {
                uint32 value;
                result = regRead(BoardTest::HEATER_WAVEFORM_CYCLES, value);
                if (result == BoardTest::OKAY) {
                    char t[16];
                    sprintf(t, "%d", value);
                    res = t;
                }
                isParsingError = false;
            }
            else if (tokens[ARGUMENT] == "waveform") {
                uint32 value;
                result = regRead(BoardTest::HEATER_CONTROL, value);
                if (result == BoardTest::OKAY) {
                    res = value & BoardTestTec::HEATER_START_WAVEFORM ? "running"  :
                          value & BoardTestTec::HEATER_STOP_WAVEFORM  ? "stopped"  :
                                                                        "undefined";
                }
                isParsingError = false;
            }
        }
        else if (tokens[ACTION] == "set" && tokens.size() > ARGUMENT) {
            if (tokens[ARGUMENT] == "tref" && tokens.size() > VALUE) {
                float refCurr;
                if (sscanf(tokens[VALUE].c_str(), "%f", &refCurr) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&refCurr);
                   result = regWrite(BoardTest::HEATER_TREF_VALUE, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::HEATER_STATUS, value);
                       if (value != LibTec::HEATER_OKAY) {
                           res = "heater status: " + heaterStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "imax" && tokens.size() > VALUE) {
                float refCurr;
                if (sscanf(tokens[VALUE].c_str(), "%f", &refCurr) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&refCurr);
                   result = regWrite(BoardTest::HEATER_IMAX_VALUE, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::HEATER_STATUS, value);
                       if (value != LibTec::HEATER_OKAY) {
                           res = "heater status: " + heaterStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "tin" && tokens.size() > VALUE) {
                std::map<std::string, int> tin;
                tin["1"] = LibTec::HEATER_T1_SELECT;
                tin["2"] = LibTec::HEATER_T2_SELECT;
                tin["3"] = LibTec::HEATER_T3_SELECT;
                tin["4"] = LibTec::HEATER_T4_SELECT;
                if (tin.find(tokens[VALUE]) != tin.end()) {
                    uint32 value = tin[tokens[VALUE]];
                    result = regWrite(BoardTest::HEATER_TIN_SELECT, value);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::HEATER_STATUS, v);
                        if (v != LibTec::HEATER_OKAY) {
                            res = "heater status: " + heaterStatus[v];
                        }
                    }
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "closedloop" && tokens.size() > VALUE) {
                if (tokens[VALUE] == "enable") {
                    result = regWrite(BoardTest::HEATER_CONTROL,
                                              BoardTestTec::HEATER_CLOSED_LOOP_ENABLE);
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "disable") {
                    result = regWrite(BoardTest::HEATER_CONTROL,
                                             BoardTestTec::HEATER_CLOSED_LOOP_DISABLE);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "propgain" && tokens.size() > VALUE) {
                float gain;
                if (sscanf(tokens[VALUE].c_str(), "%f", &gain) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&gain);
                   result = regWrite(BoardTest::HEATER_PROPORTIONAL_GAIN, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::HEATER_STATUS, value);
                       if (value != LibTec::HEATER_OKAY) {
                           res = "heater status: " + heaterStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "intgain" && tokens.size() > VALUE) {
                float gain;
                if (sscanf(tokens[VALUE].c_str(), "%f", &gain) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&gain);
                   result = regWrite(BoardTest::HEATER_INTEGRAL_GAIN, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::HEATER_STATUS, value);
                       if (value != LibTec::HEATER_OKAY) {
                           res = "heater status: " + heaterStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "dergain" && tokens.size() > VALUE) {
                float gain;
                if (sscanf(tokens[VALUE].c_str(), "%f", &gain) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&gain);
                   result = regWrite(BoardTest::HEATER_DERIVATIVE_GAIN, value);
                   if (result == BoardTest::OKAY) {
                       result = regRead(BoardTest::HEATER_STATUS, value);
                       if (value != LibTec::HEATER_OKAY) {
                           res = "heater status: " + heaterStatus[value];
                       }
                   }
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "customindex" && tokens.size() > VALUE) {
                if (tokens[VALUE] == "reset") {
                    result = regWrite(BoardTest::HEATER_CONTROL,
                                     BoardTestTec::HEATER_CUSTOM_WAVEFORM_RESET_INDEX);
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "inc") {
                    result = regWrite(BoardTest::HEATER_CONTROL,
                                       BoardTestTec::HEATER_CUSTOM_WAVEFORM_INC_INDEX);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "customtime" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "%d", &value) == 1) {
                    result = regWrite(BoardTest::HEATER_WAVEFORM_SAMPLE_TIME, value);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "customtref" && tokens.size() > VALUE) {
                float customiref;
                if (sscanf(tokens[VALUE].c_str(), "%f", &customiref) == 1) {
                   uint32 value = *reinterpret_cast<uint32*>(&customiref);
                   result = regWrite(BoardTest::HEATER_WAVEFORM_SAMPLE_TREF, value);
                   isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "customcycles" && tokens.size() > VALUE) {
                uint32 value;
                if (sscanf(tokens[VALUE].c_str(), "%d", &value) == 1) {
                    result = regWrite(BoardTest::HEATER_WAVEFORM_CYCLES, value);
                    isParsingError = false;
                }
            }
            else if (tokens[ARGUMENT] == "waveform" && tokens.size() > VALUE) {
                if (tokens[VALUE] == "start") {
                    result = regWrite(BoardTest::HEATER_CONTROL,
                                                  BoardTestTec::HEATER_START_WAVEFORM);
                    if (result == BoardTest::OKAY) {
                        uint32 v;
                        result = regRead(BoardTest::HEATER_STATUS, v);
                        if (v != LibTec::HEATER_OKAY) {
                            res = "heater status: " + heaterStatus[v];
                        }
                    }
                    isParsingError = false;
                }
                else if (tokens[VALUE] == "stop") {
                    result = regWrite(BoardTest::HEATER_CONTROL,
                                                   BoardTestTec::HEATER_STOP_WAVEFORM);
                    isParsingError = false;
                }
            }
        }
    }
    return isParsingError;
}
