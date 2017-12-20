#ifndef __THREAD_POOL__
#define __THREAD_POOL__

#include "Function.hpp"
#include "Semaphore.hpp"
#include "Promise.hpp"
#include "SpinLock.hpp"

class Thread;

class ThreadPool {
    public:
        static void initialize();
        static void destroy();

        static void launch(const Function<void()>& function);

        template<typename T>
        static Promise<T> promise(const Function<T()>& function);

    private:
        struct Task {
            Task(const Function<void()>& func);
            Function<void()> function;
            Task* next = nullptr;
        };

    private:
        static void loop();

        static void push(const Function<void()>& function);
        static void pop();

    private:
        static constexpr size_t NUM_THREADS = 16;
        static Thread** mThreads;
        static volatile bool mIsRunning;

        static Task* mTaskQueue;
        static Task** mTaskQueueEnd;
        static Semaphore mSemaphore;
        static SpinLock mLock;
};

#include "ThreadPool.inl"


#endif // __THREAD_POOL__

