#include "Semaphore.hpp"

#include <windows.h>
#include <cassert>
#include <iostream>
Semaphore::Semaphore(size_t count, unsigned short maxCount)
: mSemaphoreHandle((void**)new char[sizeof(void*)]) {
    *mSemaphoreHandle = CreateSemaphore(NULL, count, maxCount, NULL);
    assert(*mSemaphoreHandle != nullptr);
}

Semaphore::~Semaphore() {
    if(mSemaphoreHandle.getReferenceCount() == 1) {
        CloseHandle(*mSemaphoreHandle);
    }
}

void Semaphore::signal(size_t count) const {
    ReleaseSemaphore(*mSemaphoreHandle, count, NULL);
}

bool Semaphore::wait(size_t milliseconds) const {
    return WaitForSingleObject(*mSemaphoreHandle, milliseconds) == WAIT_OBJECT_0;
}

void Semaphore::wait() const {
    while(!wait(10000));
}
