#include <string.h>
#include "FreeRTOS.h"
#include "os_task.h"
#include "libAdc.h"
#include "mibspi.h"
#include "libWrapGioPort.h"
#include "libWrapMibSpi5.h"

LibAdc::LibAdc() :
    m_adcCnv(new LibWrapMibSpi5, PIN_SOMI) // 98:MIBSPI5SOMI[0]:ADC_CNV
{
}

LibAdc::~LibAdc()
{
}

int LibAdc::read(int channel, float& value)
{
    int result = startConversion(channel);
    if (result != OKAY) {
        return result;
    }
    value = getResult();
    return OKAY;
}

void LibAdc::doConversion()
{
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, true);
    vTaskDelay(pdMS_TO_TICKS(1)); // >1us
    m_adcCnv.m_libWrapGioPort->setPin(m_adcCnv.m_pin, false);
}

// RAC without BUSY indicator:
// Conversion time is max 3.2us so if we keep CNV signal high for
// more than that we disabled BUSY indicator.
// When we return CNV to low we can configure the next conv and
// read the previous  conv result. We need to repeat the conversion
// to get the up to date result
// see AD7689 Data sheet GENERAL TIMING WITHOUT A BUSY INDICATOR
int LibAdc::startConversion(int channel)
{
    switch (channel) {
    case CHANNEL_0:
    case CHANNEL_1:
    case CHANNEL_2:
    case CHANNEL_3:
    case CHANNEL_4:
    case CHANNEL_5:
        break;
    default:
        return ERROR_INVALID_CHANNEL;
    }
    m_libWrapMibSpi1.lock();
    m_libWrapMibSpi1.somiSelect(LibWrapMibSpi1::SPI_A);
    doConversion();
    uint16 cfg = 0;
    cfg |= OVERWRITE_CFG << CFG_SHIFT
        |  UNIPOLAR_REF_TO_GND << IN_CH_CFG_SHIFT
        |  channel << IN_CH_SEL_SHIFT
        |  FULL_BW << FULL_BW_SEL_SHIFT
        |  EXT_REF << REF_SEL_SHIFT
        |  DISABLE_SEQ << SEQ_EN_SHIFT
        |  READ_BACK_DISABLE << READ_BACK_SHIFT;
    m_libWrapMibSpi1.setData(LibWrapMibSpi::GROUP_0, &cfg);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi::GROUP_0);
    m_status = BUSY;
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::GROUP_0, 1)) {
        m_libWrapMibSpi1.unlock();
        m_status = ERROR_TIMEOUT;
        return OKAY;
    }
    doConversion();
    m_libWrapMibSpi1.setData(LibWrapMibSpi::GROUP_0, &cfg);
    m_libWrapMibSpi1.transfer(LibWrapMibSpi::GROUP_0);
    if (!m_libWrapMibSpi1.waitForTransferComplete(LibWrapMibSpi1::GROUP_0, 1)) {
        m_libWrapMibSpi1.unlock();
        m_status = ERROR_TIMEOUT;
        return OKAY;
    }
    uint16 result;
    m_libWrapMibSpi1.getData(LibWrapMibSpi::GROUP_0, &result);
    m_libWrapMibSpi1.unlock();
    m_status = DONE;
    m_result = result * (5.0 / 65536);
    return OKAY;
}

int LibAdc::getStatus()
{
    switch (m_status) {
    default:
    case DONE:
    case ERROR_TIMEOUT:
        m_status = IDLE;
        break;
    case BUSY:
        break;
    }
    return m_status;
}

float LibAdc::getResult()
{
    return m_result;
}
