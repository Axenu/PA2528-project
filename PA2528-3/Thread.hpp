#ifndef __THREAD__
#define __THREAD__

#include <cstring>

#include "Function.hpp"
#include "SharedPtr.hpp"
#include <utility>

class Thread {
    public:
        Thread(const Function<void()>& function);
        ~Thread();

        void run();
        void join();

        // Returns true if thread is terminated. Else false.
        bool wait(size_t milliseconds = 0) const;

    private:
        void* createThread() const;

    private:
        SharedPtr<Function<void()>> mFunction;
        SharedPtr<void*> mThreadHandle;
};

#endif // __THREAD__

