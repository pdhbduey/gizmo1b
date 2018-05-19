#include "libCircularBuffer.h"

LibCircularBuffer::LibCircularBuffer(int capacity) :
    m_capacity(capacity)
{
    clear();
}

LibCircularBuffer::~LibCircularBuffer()
{
}

int LibCircularBuffer::size()
{
    return m_size;
}

void LibCircularBuffer::clear()
{
    m_front = 0;
    m_back  = 0;
    m_size  = 0;
}

void LibCircularBuffer::pushBack()
{
    if (size() < m_capacity) {
        m_back = (m_back + 1) % m_capacity;
        m_size++;
    }
}

void LibCircularBuffer::popFront()
{
    if (m_size > 0) {
        m_front = (m_front + 1) % m_capacity;
        m_size--;
    }
}

int LibCircularBuffer::front()
{
    return m_front;
}

int LibCircularBuffer::back()
{
    return m_back;
}
