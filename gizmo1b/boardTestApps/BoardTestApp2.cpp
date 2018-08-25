#include <BoardTestApp2.h>
#include <LibLed2.h>
#include <LibCurrentDac2.h>
#include <LibAdcTempSensor2.h>
#include <LibThermistor2.h>
#include <LibThermistorOnBoard2.h>
#include <LibFault2.h>
#include <LibTec2.h>
#include <BoardTestLed.h>
#include <BoardTestAdc.h>
#include <BoardTestDac.h>
#include <BoardTestThermistor.h>
#include <BoardTestFault.h>
#include <BoardTestTec.h>
#include <LibAdcIV2.h>
#include <BoardTestAdcIV.h>

BoardTestApp2::BoardTestApp2(const char* name) :
    BoardTestApp1(name)
{
}

void BoardTestApp2::setupTests()
{
    // Create shared peripherals
    LibLed*        libLed               = new LibLed2;
    LibDac*        libCurrentDac        = new LibCurrentDac2;
    LibAdc*        libAdcTempSensor     = new LibAdcTempSensor2;
    LibAdc*        libAdcIv2            = new LibAdcIV2;
    LibThermistor* libThermistor        = new LibThermistor2(libAdcTempSensor);
    LibThermistor* libThermistorOnBoard = new LibThermistorOnBoard2(libAdcTempSensor);
    LibFault*      libFault             = new LibFault2;
    LibTec*        libTec               = new LibTec2(libCurrentDac, libAdcIv2, libThermistor);
    // Create tests
    BoardTest*      boardTestLed        = new BoardTestLed(libLed);
    BoardTest*      boardTestAdc        = new BoardTestAdc(libAdcTempSensor);
    BoardTest*      boardTestDac        = new BoardTestDac(libCurrentDac);
    BoardTest*      boardTestThermistor = new BoardTestThermistor(libThermistor, libThermistorOnBoard);
    BoardTest*      boardTestFault      = new BoardTestFault(libFault);
    BoardTest*      boardTestTec        = new BoardTestTec(libTec, libLed);
    BoardTestAdcIV* boardTestAdcIV      = new BoardTestAdcIV(libAdcIv2);
    // Map tests to test cases
    m_boardTestMap[BoardTest::LED_CONTROL]                   = boardTestLed;
    m_boardTestMap[BoardTest::ADC_CONTROL]                   = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_STATUS]                    = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_RESULT]                    = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_0]                 = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_1]                 = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_2]                 = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_3]                 = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_4]                 = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_5]                 = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_6]                 = boardTestAdc;
    m_boardTestMap[BoardTest::ADC_CHANNEL_7]                 = boardTestAdc;
    m_boardTestMap[BoardTest::DAC_VALUE]                     = boardTestDac;
    m_boardTestMap[BoardTest::DAC_STATUS]                    = boardTestDac;
    m_boardTestMap[BoardTest::THERMISTOR_STATUS]             = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_A]       = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_B]       = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_C]       = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_AIN_D]       = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_TYPE]               = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_INA1]        = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_INA2]        = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_INB1]        = boardTestThermistor;
    m_boardTestMap[BoardTest::THERMISTOR_RESULT_INB2]        = boardTestThermistor;
    m_boardTestMap[BoardTest::FAULT_RESET]                   = boardTestFault;
    m_boardTestMap[BoardTest::FAULT_STATE]                   = boardTestFault;
    m_boardTestMap[BoardTest::FAULT_NTC_PRESENT]             = boardTestFault;
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
    m_boardTestMap[BoardTest::TEC_VOUT_MAX]                  = boardTestTec;
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
    m_boardTestMap[BoardTest::ADC_IV_STATUS]                 = boardTestAdcIV;
    m_boardTestMap[BoardTest::ADC_IV_CHANNEL_0]              = boardTestAdcIV;
    m_boardTestMap[BoardTest::ADC_IV_CHANNEL_1]              = boardTestAdcIV;
}
