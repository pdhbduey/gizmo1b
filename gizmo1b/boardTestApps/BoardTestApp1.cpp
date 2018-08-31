#include <BoardTestApp1.h>
#include <libSci2.h>
#include <LibLed1.h>
#include <LibCurrentDac1.h>
#include <LibAdc1.h>
#include <LibThermistor1.h>
#include <LibFault1.h>
#include <LibTec1.h>
#include <boardTestDio.h>
#include <boardTestFan.h>
#include <boardTestMotor.h>
#include <boardTestPhotodiode.h>
#include <BoardTestAdc.h>
#include <BoardTestDac.h>
#include <BoardTestFault.h>
#include <BoardTestLed.h>
#include <BoardTestTec.h>
#include <BoardTestThermistor.h>

BoardTestApp1::BoardTestApp1(const char* name) :
    LibTask(name)
{
}

void BoardTestApp1::setupTests()
{
    // Create shared peripherals
    LibLed*        libLed        = new LibLed1;
    LibDac*        libCurrentDac = new LibCurrentDac1;
    LibAdc*        libAdc        = new LibAdc1;
    LibThermistor* libThermistor = new LibThermistor1(libAdc);
    LibFault*      libFault      = new LibFault1;
    LibTec*        libTec        = new LibTec1(libCurrentDac, libAdc, libThermistor);
    // Create tests
    BoardTest* boardTestLed        = new BoardTestLed(libLed);
    BoardTest* boardTestAdc        = new BoardTestAdc(libAdc);
    BoardTest* boardTestDac        = new BoardTestDac(libCurrentDac, libTec);
    BoardTest* boardTestFan        = new BoardTestFan;
    BoardTest* boardTestFault      = new BoardTestFault(libFault);
    BoardTest* boardTestDio        = new BoardTestDio;
    BoardTest* boardTestMotor      = new BoardTestMotor;
    BoardTest* boardTestTec        = new BoardTestTec(libTec, libLed);
    BoardTest* boardTestThermistor = new BoardTestThermistor(libThermistor);
    BoardTest* boardTestPhotodiode = new BoardTestPhotodiode;
    // Map tests to test cases
    m_boardTestMap[BoardTest::LED_CONTROL]   = boardTestLed;
    m_boardTestMap[BoardTest::ADC_CONTROL]   = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_STATUS]    = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_RESULT]    = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_0] = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_1] = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_2] = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_3] = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_4] = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_5] = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_6] = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_7] = boardTestAdc;
    m_boardTestMap[BoardTest::DAC_VALUE]  = boardTestDac;
    m_boardTestMap[BoardTest::DAC_STATUS] = boardTestDac;
    m_boardTestMap[BoardTest::FAN_PWM1_DUTY_CYCLE]   = boardTestFan;
    m_boardTestMap[BoardTest::FAN_PWM1_PERIOD_IN_US] = boardTestFan;
    m_boardTestMap[BoardTest::FAN_PWM2_DUTY_CYCLE]   = boardTestFan;
    m_boardTestMap[BoardTest::FAN_PWM2_PERIOD_IN_US] = boardTestFan;
    m_boardTestMap[BoardTest::FAN_STATUS]            = boardTestFan;
    m_boardTestMap[BoardTest::FAN_SENSOR1_RPM]       = boardTestFan;
    m_boardTestMap[BoardTest::FAN_SENSOR2_RPM]       = boardTestFan;
    m_boardTestMap[BoardTest::FAULT_RESET]       = boardTestFault;
    m_boardTestMap[BoardTest::FAULT_STATE]       = boardTestFault;
    m_boardTestMap[BoardTest::FAULT_NTC_PRESENT] = boardTestFault;
    m_boardTestMap[BoardTest::DIO_IN]  = boardTestDio;
    m_boardTestMap[BoardTest::DIO_OUT] = boardTestDio;
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
    m_boardTestMap[BoardTest::TEC_OUTPUT_LIMITER]            = boardTestTec;
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
    m_boardTestMap[BoardTest::SNAPSHOT_PERCENT_COMPLETED]    = boardTestTec;
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
    m_boardTestMap[BoardTest::THERMISTOR_STATUS]       = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_A] = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_B] = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_C] = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_D] = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_TYPE]         = boardTestThermistor;
    m_boardTestMap[BoardTest::PHOTODIODE_CONTROL]                            = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_STATUS]                             = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_INTEGRATION_TIME]                   = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_LED_INTENSITY]                      = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_READING_IN_VOLTS]                   = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_READING_RAW]                        = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_LED_BOARD_VERSION]                  = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_PD_BOARD_VERSION]                   = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_LED_TEMPERATURE]                    = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_PD_TEMPERATURE]                     = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_LED_MONITOR_PD_READING_DURING_INTEGRATION_IN_VOLTS]
                                                                             = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_LED_TEMPERATURE_DURING_INTEGRATION] = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_PD_TEMPERATURE_DURING_INTEGRATION]  = boardTestPhotodiode;
    m_boardTestMap[BoardTest::PHOTODIODE_LED_MONITOR_PD_READING_IN_VOLTS]    = boardTestPhotodiode;
}

void BoardTestApp1::run()
{
    setupTests();
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

int BoardTestApp1::regRead(uint32 address, uint32& value)
{
    int result = BoardTestApp::regRead(address, value);
    if (result != BoardTest::ERROR_ADDR) {
        return result;
    }
    else if (isAddressValidTecSnapshotRange(address)) {
        return m_boardTestMap[BoardTest::SNAPSHOT_TEC_SAMPLES_RANGE]->get(address, value);
    }
    return BoardTest::ERROR_ADDR;
}

int BoardTestApp1::regWrite(uint32 address, uint32 value)
{
    int result = BoardTestApp::regWrite(address, value);
    if (result != BoardTest::ERROR_ADDR) {
        return result;
    }
    else if (isAddressValidTecSnapshotRange(address)) {
        return m_boardTestMap[BoardTest::SNAPSHOT_TEC_SAMPLES_RANGE]->set(address, value);
    }
    return BoardTest::ERROR_ADDR;
}

void BoardTestApp1::decodeMessage(std::vector<uint8>& message,
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

bool BoardTestApp1::isAddressValidTecSnapshotRange(uint32 address)
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
