#ifndef _LIB_CIRCULAR_BUFFER_H_
#define _LIB_CIRCULAR_BUFFER_H_

class LibCircularBuffer
{
public:
    LibCircularBuffer(int capacity);
    virtual ~LibCircularBuffer();
    int size();
    void clear();
    void pushBack();
    void popFront();
    int front();
    int back();
private:
    int m_front;
    int m_back;
    int m_capacity;
    int m_size;
};

#endif // _LIB_CIRCULAR_BUFFER_H_
