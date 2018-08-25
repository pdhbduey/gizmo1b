#ifndef _BOARD_TEST_PHOTODIODE_H_
#define _BOARD_TEST_PHOTODIODE_H_

#include "BoardTest.h"
#include "libPhotodiode.h"

class BoardTestPhotodiode: public BoardTest
{
public:
    BoardTestPhotodiode();
    virtual ~BoardTestPhotodiode();
    virtual int get(uint32 address, uint32& value);
    virtual int set(uint32 address, uint32 value);
private:
    LibPhotodiode m_libPhotodiode;
    int m_status;
};

#endif // _BOARD_TEST_PHOTODIODE_H_
