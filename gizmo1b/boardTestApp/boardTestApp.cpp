#include "boardTestApp.h"
#include "libSci2.h"
#include "boardTestDio.h"
#include "boardTestAdc.h"
#include "boardTestDac.h"
#include "boardTestFan.h"
#include "boardTestFault.h"
#include "boardTestMotor.h"
#include "boardTestTec.h"
#include "boardTestThermistor.h"
#include "boardTestLed.h"

BoardTestApp::BoardTestApp(const char* name) :
    LibTask(name)
{
    BoardTest* boardTest = new BoardTestAdc;
    m_boardTestMap[BoardTest::ADC_CONTROL] = boardTest;
    m_boardTestMap[BoardTest::ADC_STATUS]  = boardTest;
    m_boardTestMap[BoardTest::ADC_RESULT]  = boardTest;
    boardTest = new BoardTestDac;
    m_boardTestMap[BoardTest::DAC_VALUE] = boardTest;
    m_boardTestMap[BoardTest::DAC_STATUS] = boardTest;
    m_boardTestMap[BoardTest::FAN_VALUE] = new BoardTestFan;
    boardTest = new BoardTestFault;
    m_boardTestMap[BoardTest::FAULT_RESET] = boardTest;
    m_boardTestMap[BoardTest::FAULT_STATE] = boardTest;
    boardTest = new BoardTestDio;
    m_boardTestMap[BoardTest::DIO_IN]  = boardTest;
    m_boardTestMap[BoardTest::DIO_OUT] = boardTest;
    boardTest = new BoardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_CONTROL]           = boardTest;
    m_boardTestMap[BoardTest::MOTOR_STATUS]            = boardTest;
    m_boardTestMap[BoardTest::MOTOR_RELATIVE_POSITION] = boardTest;
    m_boardTestMap[BoardTest::MOTOR_ABSOLUTE_POSITION] = boardTest;
    m_boardTestMap[BoardTest::MOTOR_SPEED]             = boardTest;
    m_boardTestMap[BoardTest::MOTOR_ACCELERATION]      = boardTest;
    m_boardTestMap[BoardTest::MOTOR_DECELERATION]      = boardTest;
    m_boardTestMap[BoardTest::MOTOR_HOLD_CURRENT]      = boardTest;
    boardTest = new BoardTestTec;
    m_boardTestMap[BoardTest::TEC_CONTROL]            = boardTest;
    m_boardTestMap[BoardTest::TEC_TIME_VALUE]         = boardTest;
    m_boardTestMap[BoardTest::TEC_CURRENT_VALUE]      = boardTest;
    m_boardTestMap[BoardTest::TEC_CURRENT_COUNT]      = boardTest;
    m_boardTestMap[BoardTest::TEC_CURRENT_LOOP_COUNT] = boardTest;
    m_boardTestMap[BoardTest::TEC_ISENSE_VALUE]       = boardTest;
    m_boardTestMap[BoardTest::TEC_VSENSE_VALUE]       = boardTest;
    m_boardTestMap[BoardTest::TEC_LOOP_VALUE]         = boardTest;
    m_boardTestMap[BoardTest::TEC_COUNT_VALUE]        = boardTest;
    boardTest = new BoardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_CONTROL] = boardTest;
    m_boardTestMap[BoardTest::THERMISTOR_STATUS]  = boardTest;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT]  = boardTest;
    m_boardTestMap[BoardTest::LED_CONTROL] = new BoardTestLed;
}

BoardTestApp::~BoardTestApp()
{
}

void BoardTestApp::run()
{
    LibSci2 libSci(32, 32);
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
        if (!libSci.waitForReadyRead(1000)) {
            continue;
        }
        message.clear();
        libSci.read(message);
        while (message.size() < sizeof(m_masterToSlave)) {
            if (!libSci.waitForReadyRead(10)) {
                resetSci = true;
                break;
            }
            libSci.read(message);
        }
        if (!resetSci) {
            decodeMessage(message, response);
            if (libSci.write(response) == BoardTest::OKAY) {
                if (!libSci.waitForBytesWritten(1000)) {
                    resetSci = true;
                }
            }
            else {
                resetSci = true;
            }
        }
    }
}

void BoardTestApp::decodeMessage(std::vector<uint8>& message,
                                                   std::vector<uint8>& response)
{
    uint8* buffer = m_masterToSlave.buffer;
    for (int i = 0; i < sizeof(m_masterToSlave); i++) {
        buffer[i] = message[i];
    }
    struct MasterToSlaveMsg::msgMasterToSlave& msgMtoS = m_masterToSlave.msg;
    uint8 command = msgMtoS.command;
    uint32 address = msgMtoS.address[0] << 24
                   | msgMtoS.address[1] << 16
                   | msgMtoS.address[2] << 8
                   | msgMtoS.address[3];
    uint32 value = msgMtoS.data[0] << 24
                 | msgMtoS.data[1] << 16
                 | msgMtoS.data[2] << 8
                 | msgMtoS.data[3];
    uint32 status;
    switch (command) {
    default:
        status = BoardTest::ERROR_CMD;
        break;
    case BoardTest::REG_READ:
        status = regRead(address, value);
        break;
    case BoardTest::REG_WRITE:
        status = regWrite(address, value);
        break;
    }
    struct SlaveToMasterMsg::msgSlaveToMaster& msgStoM = m_slaveToMaster.msg;
    msgStoM.status  = status;
    msgStoM.data[0] = value >> 24;
    msgStoM.data[1] = value >> 16;
    msgStoM.data[2] = value >>  8;
    msgStoM.data[3] = value;
    buffer = m_slaveToMaster.buffer;
    response.clear();
    for (int i = 0; i < sizeof(m_slaveToMaster); i++) {
        response.push_back(buffer[i]);
    }
}

bool BoardTestApp::isAddressValid(uint32 address)
{
    return m_boardTestMap.find(address) != m_boardTestMap.end()
        && m_boardTestMap[address];
}

int BoardTestApp::regRead(uint32 address, uint32& value)
{
    if (!isAddressValid(address)) {
        return BoardTest::ERROR_ADDR;
    }
    return m_boardTestMap[address]->get(address, value);
}

int BoardTestApp::regWrite(uint32 address, uint32 value)
{
    if (!isAddressValid(address)) {
        return BoardTest::ERROR_ADDR;
    }
    return m_boardTestMap[address]->set(address, value);
}
