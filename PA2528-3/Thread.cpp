#include "Thread.hpp"

#include <windows.h>
#include <cassert>
#include <utility>

Thread::Thread(const Function<void()>& function)
: mFunction(new Function<void()>(function))
, mThreadHandle((void**)new char[sizeof(void*)]()){
    *mThreadHandle = nullptr;
}

Thread::~Thread() {
    join();
}

void Thread::run() {
    join();
    *mThreadHandle = createThread();
    assert(*mThreadHandle != nullptr);
}

void Thread::join() {
    if(!*mThreadHandle) {
        return;
    }

    while(!wait(1000));

    CloseHandle(*mThreadHandle);
    *mThreadHandle = nullptr;
}

bool Thread::wait(size_t milliseconds) const {
    if(!*mThreadHandle) {
        return true;
    }
    return WaitForSingleObject(*mThreadHandle, milliseconds) == WAIT_OBJECT_0;
}

namespace ThreadPrivate {
    DWORD WINAPI entry(void* args) {
        ((Function<void()>*)args)->operator()();
        return 0;
    }
}

void* Thread::createThread() const {

    void* h = CreateThread(NULL, 0, ThreadPrivate::entry, (void*)mFunction.get(), 0, NULL);
    if(h == NULL) {
        return nullptr;
    }
    return h;
}
