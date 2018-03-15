#ifndef _LIB_DAC_H_
#define _LIB_DAC_H_

class LibDac
{
public:
    enum Status {
        OKAY,
        ERROR_SET_VALUE_OUT_OF_RANGE,
    };
    LibDac();
    virtual ~LibDac();
    int set(float value); // 0-5V
};

#endif // _LIB_DAC_H_
