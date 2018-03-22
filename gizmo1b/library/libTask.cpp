#include "libTask.h"

LibTask::LibTask(const char* name, uint16_t stackSize, UBaseType_t priority) :
    m_name(name),
    m_stackSize(stackSize),
    m_priority(priority)
{
}

LibTask::~LibTask()
{
    if (!m_taskHandle) {
        vTaskDelete(m_taskHandle);
    }
}

void LibTask::start()
{
    UBaseType_t priority = m_priority | portPRIVILEGE_BIT;
    xTaskCreate(task, m_name.c_str(), m_stackSize, this, priority,
                                                                 &m_taskHandle);
}

void LibTask::task(void* parameter)
{
    reinterpret_cast<LibTask*>(parameter)->run();
    reinterpret_cast<LibTask*>(parameter)->m_taskHandle = NULL;
    vTaskDelete(NULL);
}
