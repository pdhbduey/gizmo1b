#include <LibAdc.h>

LibAdc::LibAdc(float vref) :
    m_vref(vref)
{
}

bool LibAdc::isChannelCorrect(int channel)
{
    int result = OKAY;
    switch (channel) {
    default:
        result = ERROR_INVALID_CHANNEL;
    case CHANNEL_0:
    case CHANNEL_1:
    case CHANNEL_2:
    case CHANNEL_3:
    case CHANNEL_4:
    case CHANNEL_5:
    case CHANNEL_6:
    case CHANNEL_7:
        break;
    }
    return result;
}

int LibAdc::read(int channel, float& value)
{
    uint32 data;
    int result = read(channel, data);
    if (result == OKAY) {
        value = data * (m_vref / 65536);
    }
    return result;
}
