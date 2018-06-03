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
#include "boardTestPhotodiode.h"
#include "boardTestApp.h"

BoardTestApp::BoardTestApp(const char* name) :
    LibTask(name)
{
    BoardTest* boardTestAdc                = new BoardTestAdc;
    m_boardTestMap[BoardTest::ADC_CONTROL] = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_STATUS]  = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_RESULT]  = boardTestAdc;
    BoardTest* boardTestDac               = new BoardTestDac;
    m_boardTestMap[BoardTest::DAC_VALUE]  = boardTestDac;
    m_boardTestMap[BoardTest::DAC_STATUS] = boardTestDac;
    BoardTest* boardTestFan                          = new BoardTestFan;
    m_boardTestMap[BoardTest::FAN_PWM1_DUTY_CYCLE]   = boardTestFan;
    m_boardTestMap[BoardTest::FAN_PWM1_PERIOD_IN_US] = boardTestFan;
    m_boardTestMap[BoardTest::FAN_PWM2_DUTY_CYCLE]   = boardTestFan;
    m_boardTestMap[BoardTest::FAN_PWM2_PERIOD_IN_US] = boardTestFan;
    m_boardTestMap[BoardTest::FAN_STATUS]            = boardTestFan;
    m_boardTestMap[BoardTest::FAN_SENSOR1_RPM]       = boardTestFan;
    m_boardTestMap[BoardTest::FAN_SENSOR2_RPM]       = boardTestFan;
    BoardTest* boardTestFault                    = new BoardTestFault;
    m_boardTestMap[BoardTest::FAULT_RESET]       = boardTestFault;
    m_boardTestMap[BoardTest::FAULT_STATE]       = boardTestFault;
    m_boardTestMap[BoardTest::FAULT_NTC_PRESENT] = boardTestFault;
    BoardTest* boardTestDio            = new BoardTestDio;
    m_boardTestMap[BoardTest::DIO_IN]  = boardTestDio;
    m_boardTestMap[BoardTest::DIO_OUT] = boardTestDio;
    BoardTest* boardTestMotor                             = new BoardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_CONTROL]              = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_COMMUNICATION_STATUS] = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_RELATIVE_POSITION]    = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_ABSOLUTE_POSITION]    = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_SPEED]                = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_ACCELERATION]         = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_DECELERATION]         = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_CURRENT_POSITION]     = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_STATUS]               = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_REGISTER_ADDRESS]     = boardTestMotor;
    m_boardTestMap[BoardTest::MOTOR_REGISTER_VALUE]       = boardTestMotor;
    BoardTest* boardTestTec                                  = new BoardTestTec;
    m_boardTestMap[BoardTest::TEC_CONTROL]                   = boardTestTec;
    m_boardTestMap[BoardTest::TEC_IREF_VALUE]                = boardTestTec;
    m_boardTestMap[BoardTest::TEC_ISENSE_VALUE]              = boardTestTec;
    m_boardTestMap[BoardTest::TEC_VSENSE_VALUE]              = boardTestTec;
    m_boardTestMap[BoardTest::TEC_STATUS]                    = boardTestTec;
    m_boardTestMap[BoardTest::TEC_WAVEFORM_TYPE]             = boardTestTec;
    m_boardTestMap[BoardTest::TEC_WAVEFORM_PERIOD]           = boardTestTec;
    m_boardTestMap[BoardTest::TEC_PROPORTIONAL_GAIN]         = boardTestTec;
    m_boardTestMap[BoardTest::TEC_INTEGRAL_GAIN]             = boardTestTec;
    m_boardTestMap[BoardTest::TEC_DERIVATIVE_GAIN]           = boardTestTec;
    m_boardTestMap[BoardTest::TEC_WAVEFORM_SAMPLE_INDEX]     = boardTestTec;
    m_boardTestMap[BoardTest::TEC_WAVEFORM_SAMPLE_TIME]      = boardTestTec;
    m_boardTestMap[BoardTest::TEC_WAVEFORM_SAMPLE_IREF]      = boardTestTec;
    m_boardTestMap[BoardTest::TEC_WAVEFORM_CYCLES]           = boardTestTec;
    m_boardTestMap[BoardTest::SNAPSHOT_CONTROL]              = boardTestTec;
    m_boardTestMap[BoardTest::SNAPSHOT_STATUS]               = boardTestTec;
    m_boardTestMap[BoardTest::SNAPSHOT_RESOLUTION]           = boardTestTec;
    m_boardTestMap[BoardTest::SNAPSHOT_NUMBER_OF_SAMPLES]    = boardTestTec;
    m_boardTestMap[BoardTest::SNAPSHOT_TEC_SAMPLES_RANGE]    = boardTestTec;
    m_boardTestMap[BoardTest::TRACE_CONTROL]                 = boardTestTec;
    m_boardTestMap[BoardTest::TRACE_STATUS]                  = boardTestTec;
    m_boardTestMap[BoardTest::TRACE_RESOLUTION]              = boardTestTec;
    m_boardTestMap[BoardTest::TRACE_FIRST_SAMPLE]            = boardTestTec;
    m_boardTestMap[BoardTest::TRACE_NUMBER_OF_SAMPLES]       = boardTestTec;
    m_boardTestMap[BoardTest::TRACE_NUMBER_OF_READ_SAMPLES]  = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_CONTROL]                = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_STATUS]                 = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_TREF_VALUE]             = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_IMAX_VALUE]             = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_TIN_SELECT]             = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_PROPORTIONAL_GAIN]      = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_INTEGRAL_GAIN]          = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_DERIVATIVE_GAIN]        = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_WAVEFORM_SAMPLE_INDEX]  = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_WAVEFORM_SAMPLE_TIME]   = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_WAVEFORM_SAMPLE_TREF]   = boardTestTec;
    m_boardTestMap[BoardTest::HEATER_WAVEFORM_CYCLES]        = boardTestTec;
    BoardTest* boardTestThermistor                     = new BoardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_STATUS]       = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_A] = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_B] = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_C] = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_D] = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_TYPE]         = boardTestThermistor;
    m_boardTestMap[BoardTest::LED_CONTROL] = new BoardTestLed;
    BoardTest* boardTestPhotodiode                         = new BoardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_CONTROL]          = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_STATUS]           = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_INTEGRATION_TIME] = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_LED_INTENSITY]    = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_READING]          = boardTestPhotodiode;
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
        if (!libSci.isDataAvailable(1000)) {
            continue;
        }
        message.clear();
        libSci.read(message);
        while (message.size() < sizeof(m_masterToSlave)) {
            if (!libSci.isDataAvailable(10)) {
                resetSci = true;
                break;
            }
            libSci.read(message);
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

bool BoardTestApp::isAddressValidTecSnapshotRange(uint32 address)
{
    return (address >= BoardTest::SNAPSHOT_SAMPLES_VSENSE
        &&  address <= BoardTest::SNAPSHOT_SAMPLES_VSENSE_MAX)
        || (address >= BoardTest::SNAPSHOT_SAMPLES_ISENSE
        &&  address <= BoardTest::SNAPSHOT_SAMPLES_ISENSE_MAX)
        || (address >= BoardTest::SNAPSHOT_SAMPLES_IREF
        &&  address <= BoardTest::SNAPSHOT_SAMPLES_IREF_MAX)
        || (address >= BoardTest::SNAPSHOT_SAMPLES_TEMPERATURE1
        &&  address <= BoardTest::SNAPSHOT_SAMPLES_TEMPERATURE1_MAX)
        || (address >= BoardTest::SNAPSHOT_SAMPLES_TEMPERATURE2
        &&  address <= BoardTest::SNAPSHOT_SAMPLES_TEMPERATURE2_MAX)
        || (address >= BoardTest::SNAPSHOT_SAMPLES_TEMPERATURE3
        &&  address <= BoardTest::SNAPSHOT_SAMPLES_TEMPERATURE3_MAX)
        || (address >= BoardTest::SNAPSHOT_SAMPLES_TEMPERATURE4
        &&  address <= BoardTest::SNAPSHOT_SAMPLES_TEMPERATURE4_MAX);
}

bool BoardTestApp::isAddressValid(uint32 address)
{
    return (m_boardTestMap.find(address) != m_boardTestMap.end()
        && m_boardTestMap[address]);
}

int BoardTestApp::regRead(uint32 address, uint32& value)
{
    if (isAddressValid(address)) {
        return m_boardTestMap[address]->get(address, value);
    }
    else if (isAddressValidTecSnapshotRange(address)) {
        return m_boardTestMap[BoardTest::SNAPSHOT_TEC_SAMPLES_RANGE]->get(address, value);
    }
    return BoardTest::ERROR_ADDR;
}

int BoardTestApp::regWrite(uint32 address, uint32 value)
{
    if (isAddressValid(address)) {
        return m_boardTestMap[address]->set(address, value);
    }
    else if (isAddressValidTecSnapshotRange(address)) {
        return m_boardTestMap[BoardTest::SNAPSHOT_TEC_SAMPLES_RANGE]->set(address, value);
    }
    return BoardTest::ERROR_ADDR;
}
