#include "libMutex.h"
#include "libThermistor.h"

SemaphoreHandle_t LibThermistor::s_mutex;
bool LibThermistor::s_isInitialized;
struct LibThermistor::conversion LibThermistor::s_convTable[] = {
    { .rt = 336479.00, .temp = { -40, -40.0 } },
    { .rt = 314904.00, .temp = { -39, -38.2 } },
    { .rt = 294848.00, .temp = { -38, -36.4 } },
    { .rt = 276194.00, .temp = { -37, -34.6 } },
    { .rt = 258838.00, .temp = { -36, -32.8 } },
    { .rt = 242681.00, .temp = { -35, -31.0 } },
    { .rt = 227632.00, .temp = { -34, -29.2 } },
    { .rt = 213610.00, .temp = { -33, -27.4 } },
    { .rt = 200539.00, .temp = { -32, -25.6 } },
    { .rt = 188349.00, .temp = { -31, -23.8 } },
    { .rt = 176974.00, .temp = { -30, -22.0 } },
    { .rt = 166356.00, .temp = { -29, -20.2 } },
    { .rt = 156441.00, .temp = { -28, -18.4 } },
    { .rt = 147177.00, .temp = { -27, -16.6 } },
    { .rt = 138518.00, .temp = { -26, -14.8 } },
    { .rt = 130421.00, .temp = { -25, -13.0 } },
    { .rt = 122847.00, .temp = { -24, -11.2 } },
    { .rt = 115759.00, .temp = { -23,  -9.4 } },
    { .rt = 109122.00, .temp = { -22,  -7.6 } },
    { .rt = 102906.00, .temp = { -21,  -5.8 } },
    { .rt =  97081.00, .temp = { -20,  -4.0 } },
    { .rt =  91621.00, .temp = { -19,  -2.2 } },
    { .rt =  86501.00, .temp = { -18,  -0.4 } },
    { .rt =  81698.00, .temp = { -17,   1.4 } },
    { .rt =  77190.00, .temp = { -16,   3.2 } },
    { .rt =  72957.00, .temp = { -15,   5.0 } },
    { .rt =  68982.00, .temp = { -14,   6.8 } },
    { .rt =  65246.00, .temp = { -13,   8.6 } },
    { .rt =  61736.00, .temp = { -12,  10.4 } },
    { .rt =  58434.00, .temp = { -11,  12.2 } },
    { .rt =  55329.00, .temp = { -10,  14.0 } },
    { .rt =  52407.00, .temp = {  -9,  15.8 } },
    { .rt =  49656.00, .temp = {  -8,  17.6 } },
    { .rt =  47066.00, .temp = {  -7,  19.4 } },
    { .rt =  44626.00, .temp = {  -6,  21.2 } },
    { .rt =  42327.00, .temp = {  -5,  23.0 } },
    { .rt =  40159.00, .temp = {  -4,  24.8 } },
    { .rt =  38115.00, .temp = {  -3,  26.6 } },
    { .rt =  36187.00, .temp = {  -2,  28.4 } },
    { .rt =  34368.00, .temp = {  -1,  30.2 } },
    { .rt =  32650.00, .temp = {   0,  32.0 } },
    { .rt =  31029.00, .temp = {   1,  33.8 } },
    { .rt =  29498.00, .temp = {   2,  35.6 } },
    { .rt =  28052.00, .temp = {   3,  37.4 } },
    { .rt =  26685.00, .temp = {   4,  39.2 } },
    { .rt =  25392.00, .temp = {   5,  41.0 } },
    { .rt =  24170.00, .temp = {   6,  42.8 } },
    { .rt =  23013.00, .temp = {   7,  44.6 } },
    { .rt =  21918.00, .temp = {   8,  46.4 } },
    { .rt =  20882.00, .temp = {   9,  48.2 } },
    { .rt =  19901.00, .temp = {  10,  50.0 } },
    { .rt =  18971.00, .temp = {  11,  51.8 } },
    { .rt =  18090.00, .temp = {  12,  53.6 } },
    { .rt =  17255.00, .temp = {  13,  55.4 } },
    { .rt =  16463.00, .temp = {  14,  57.2 } },
    { .rt =  15712.00, .temp = {  15,  59.0 } },
    { .rt =  14999.00, .temp = {  16,  60.8 } },
    { .rt =  14323.00, .temp = {  17,  62.6 } },
    { .rt =  13681.00, .temp = {  18,  64.4 } },
    { .rt =  13072.00, .temp = {  19,  66.2 } },
    { .rt =  12493.00, .temp = {  20,  68.0 } },
    { .rt =  11942.00, .temp = {  21,  69.8 } },
    { .rt =  11419.00, .temp = {  22,  71.6 } },
    { .rt =  10922.00, .temp = {  23,  73.4 } },
    { .rt =  10450.00, .temp = {  24,  75.2 } },
    { .rt =  10000.00, .temp = {  25,  77.0 } },
    { .rt =   9572.00, .temp = {  26,  78.8 } },
    { .rt =   9165.00, .temp = {  27,  80.6 } },
    { .rt =   8777.00, .temp = {  28,  82.4 } },
    { .rt =   8408.00, .temp = {  29,  84.2 } },
    { .rt =   8057.00, .temp = {  30,  86.0 } },
    { .rt =   7722.00, .temp = {  31,  87.8 } },
    { .rt =   7402.00, .temp = {  32,  89.6 } },
    { .rt =   7098.00, .temp = {  33,  91.4 } },
    { .rt =   6808.00, .temp = {  34,  93.2 } },
    { .rt =   6531.00, .temp = {  35,  95.0 } },
    { .rt =   6267.00, .temp = {  36,  96.8 } },
    { .rt =   6015.00, .temp = {  37,  98.6 } },
    { .rt =   5775.00, .temp = {  38, 100.4 } },
    { .rt =   5545.00, .temp = {  39, 102.2 } },
    { .rt =   5326.00, .temp = {  40, 104.0 } },
    { .rt =   5117.00, .temp = {  41, 105.8 } },
    { .rt =   4917.00, .temp = {  42, 107.6 } },
    { .rt =   4725.00, .temp = {  43, 109.4 } },
    { .rt =   4543.00, .temp = {  44, 111.2 } },
    { .rt =   4368.00, .temp = {  45, 113.0 } },
    { .rt =   4201.00, .temp = {  46, 114.8 } },
    { .rt =   4041.00, .temp = {  47, 116.6 } },
    { .rt =   3888.00, .temp = {  48, 118.4 } },
    { .rt =   3742.00, .temp = {  49, 120.2 } },
    { .rt =   3602.00, .temp = {  50, 122.0 } },
    { .rt =   3468.00, .temp = {  51, 123.8 } },
    { .rt =   3340.00, .temp = {  52, 125.6 } },
    { .rt =   3217.00, .temp = {  53, 127.4 } },
    { .rt =   3099.00, .temp = {  54, 129.2 } },
    { .rt =   2986.00, .temp = {  55, 131.0 } },
    { .rt =   2878.00, .temp = {  56, 132.8 } },
    { .rt =   2774.00, .temp = {  57, 134.6 } },
    { .rt =   2675.00, .temp = {  58, 136.4 } },
    { .rt =   2579.00, .temp = {  59, 138.2 } },
    { .rt =   2488.00, .temp = {  60, 140.0 } },
    { .rt =   2400.00, .temp = {  61, 141.8 } },
    { .rt =   2316.00, .temp = {  62, 143.6 } },
    { .rt =   2235.00, .temp = {  63, 145.4 } },
    { .rt =   2157.00, .temp = {  64, 147.2 } },
    { .rt =   2083.00, .temp = {  65, 149.0 } },
    { .rt =   2011.00, .temp = {  66, 150.8 } },
    { .rt =   1942.00, .temp = {  67, 152.6 } },
    { .rt =   1876.00, .temp = {  68, 154.4 } },
    { .rt =   1813.00, .temp = {  69, 156.2 } },
    { .rt =   1752.00, .temp = {  70, 158.0 } },
    { .rt =   1693.00, .temp = {  71, 159.8 } },
    { .rt =   1637.00, .temp = {  72, 161.6 } },
    { .rt =   1582.00, .temp = {  73, 163.4 } },
    { .rt =   1530.00, .temp = {  74, 165.2 } },
    { .rt =   1480.00, .temp = {  75, 167.0 } },
    { .rt =   1432.00, .temp = {  76, 168.8 } },
    { .rt =   1385.00, .temp = {  77, 170.6 } },
    { .rt =   1340.00, .temp = {  78, 172.4 } },
    { .rt =   1297.00, .temp = {  79, 174.2 } },
    { .rt =   1255.00, .temp = {  80, 176.0 } },
    { .rt =   1215.00, .temp = {  81, 177.8 } },
    { .rt =   1177.00, .temp = {  82, 179.6 } },
    { .rt =   1140.00, .temp = {  83, 181.4 } },
    { .rt =   1104.00, .temp = {  84, 183.2 } },
    { .rt =   1070.00, .temp = {  85, 185.0 } },
    { .rt =   1037.00, .temp = {  86, 186.8 } },
    { .rt =   1005.00, .temp = {  87, 188.6 } },
    { .rt =    973.80, .temp = {  88, 190.4 } },
    { .rt =    944.10, .temp = {  89, 192.2 } },
    { .rt =    915.50, .temp = {  90, 194.0 } },
    { .rt =    887.80, .temp = {  91, 195.8 } },
    { .rt =    861.20, .temp = {  92, 197.6 } },
    { .rt =    835.40, .temp = {  93, 199.4 } },
    { .rt =    810.60, .temp = {  94, 201.2 } },
    { .rt =    786.60, .temp = {  95, 203.0 } },
    { .rt =    763.50, .temp = {  96, 204.8 } },
    { .rt =    741.20, .temp = {  97, 206.6 } },
    { .rt =    719.60, .temp = {  98, 208.4 } },
    { .rt =    698.70, .temp = {  99, 210.2 } },
    { .rt =    678.60, .temp = { 100, 212.0 } },
    { .rt =    659.10, .temp = { 101, 213.8 } },
    { .rt =    640.30, .temp = { 102, 215.6 } },
    { .rt =    622.20, .temp = { 103, 217.4 } },
    { .rt =    604.60, .temp = { 104, 219.2 } },
    { .rt =    587.60, .temp = { 105, 221.0 } },
    { .rt =    571.20, .temp = { 106, 222.8 } },
    { .rt =    555.30, .temp = { 107, 224.6 } },
    { .rt =    539.90, .temp = { 108, 226.4 } },
    { .rt =    525.00, .temp = { 109, 228.2 } },
    { .rt =    510.60, .temp = { 110, 230.0 } },
    { .rt =    496.70, .temp = { 111, 231.8 } },
    { .rt =    483.20, .temp = { 112, 233.6 } },
    { .rt =    470.10, .temp = { 113, 235.4 } },
    { .rt =    457.50, .temp = { 114, 237.2 } },
    { .rt =    445.30, .temp = { 115, 239.0 } },
    { .rt =    433.40, .temp = { 116, 240.8 } },
    { .rt =    421.90, .temp = { 117, 242.6 } },
    { .rt =    410.80, .temp = { 118, 244.4 } },
    { .rt =    400.00, .temp = { 119, 246.2 } },
    { .rt =    389.60, .temp = { 120, 248.0 } },
    { .rt =    379.40, .temp = { 121, 249.8 } },
    { .rt =    369.60, .temp = { 122, 251.6 } },
    { .rt =    360.10, .temp = { 123, 253.4 } },
    { .rt =    350.90, .temp = { 124, 255.2 } },
    { .rt =    341.90, .temp = { 125, 257.0 } },
    { .rt =    0,                             },
};


LibThermistor::LibThermistor()
{
    if (!s_isInitialized) {
        s_mutex = xSemaphoreCreateMutex();
        s_isInitialized = true;
    }
}

LibThermistor::~LibThermistor()
{
}

// TEMP_AINx = 2.048 * Rt/(107000 + Rt)
// 1/TEMP_AINx = (107000 / Rt + 1) / 2.048
// 1 / Rt = (2.048 /TEMP_AINx - 1) / 107000
float LibThermistor::convertVoltageToTemp(float ain, int standard)
{
    float rt = 107000 / (2.048 / ain - 1);
    int i;
    float temp;
    if (rt > s_convTable[0].rt) {
        return s_convTable[0].temp[standard];
    }
    for (i = 0; s_convTable[i].rt; i++) {
        if (s_convTable[i].rt >= rt && rt >= s_convTable[i + 1].rt) {
            if (s_convTable[i + 1].rt == 0) {
                temp = s_convTable[i].temp[standard];
                break;
            }
            else {
                temp = s_convTable[i + 1].temp[standard]
                  + (rt - s_convTable[i + 1].rt)
                  * (s_convTable[i].temp[standard] - s_convTable[i + 1].temp[standard])
                  / (s_convTable[i].rt - s_convTable[i + 1].rt);
                 break;
            }
        }
    }
    if (!s_convTable[i].rt) {
        return s_convTable[i].temp[standard];
    }
    return temp;
}

int LibThermistor::readTemp(int channel, float& value)
{
    LibMutex libMutex(s_mutex);
    int result = OKAY;
    int adcChannel;
    switch (channel) {
    case AIN_A:
        adcChannel = TEMP_AIN_A;
        break;
    case AIN_B:
        adcChannel = TEMP_AIN_B;
        break;
    case AIN_C:
        adcChannel = TEMP_AIN_C;
        break;
    case AIN_D:
        adcChannel = TEMP_AIN_D;
        break;
    default:
        return ERROR_INVALID_TEMP_CHANNEL;
    }
    float ain;
    result = m_libAdc.read(adcChannel, ain);
    if (result != LibAdc::OKAY) {
        switch (channel) {
        case AIN_A:
            result = ERROR_READ_TEMP_AIN_A;
            break;
        case AIN_B:
            result = ERROR_READ_TEMP_AIN_B;
            break;
        case AIN_C:
            result = ERROR_READ_TEMP_AIN_C;
            break;
        case AIN_D:
            result = ERROR_READ_TEMP_AIN_D;
            break;
        }
        return result;
    }
    value = convertVoltageToTemp(ain, CELSIUS);
    return OKAY;
}

void LibThermistor::test()
{
    LibThermistor libThermistor;
    float ainTempA;
    int result = libThermistor.readTemp(AIN_A, ainTempA);
    float ainTempB;
    result = libThermistor.readTemp(AIN_B, ainTempB);
    float ainTempC;
    result = libThermistor.readTemp(AIN_C, ainTempC);
    float ainTempD;
    result = libThermistor.readTemp(AIN_D, ainTempD);
}
