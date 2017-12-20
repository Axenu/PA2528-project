#include "ThreadPool.hpp"
#include "Thread.hpp"

#include <cassert>

Thread** ThreadPool::mThreads = nullptr;
volatile bool ThreadPool::mIsRunning = false;
ThreadPool::Task* ThreadPool::mTaskQueue = nullptr;
ThreadPool::Task** ThreadPool::mTaskQueueEnd = &ThreadPool::mTaskQueue;
Semaphore ThreadPool::mSemaphore;
SpinLock ThreadPool::mLock;

void ThreadPool::initialize() {
    static bool isInitialized = false;
    if(isInitialized) {
        return;
    }
    isInitialized = true;

    mIsRunning = true;
    mThreads = new Thread*[NUM_THREADS];
    for(size_t i = 0; i < NUM_THREADS; i++) {
        mThreads[i] = new Thread(loop);
        mThreads[i]->run();
    }
}

void ThreadPool::destroy() {
    mIsRunning = false;
    mSemaphore.signal(NUM_THREADS);
    for(size_t i = 0; i < NUM_THREADS; i++) {
        delete mThreads[i];
    }
    delete mThreads;
}

void ThreadPool::launch(const Function<void()>& function) {
    push(function);
}

void ThreadPool::loop() {
    while(mIsRunning) {
        pop();
    }
}

void ThreadPool::push(const Function<void()>& function) {
    Task* task = new Task(function);

    mLock.lock();
    *mTaskQueueEnd = task;
    mTaskQueueEnd = &task->next;
    mLock.unlock();

    mSemaphore.signal();
}

void ThreadPool::pop() {
    if(!mSemaphore.wait(100)) {
        return;
    }
    mLock.lock();
    assert(mTaskQueue);
    Task* task = mTaskQueue;
    mTaskQueue = task->next;
    if(!mTaskQueue) {
        mTaskQueueEnd = &mTaskQueue;
    }
    mLock.unlock();

    task->function();
    delete task;
}



ThreadPool::Task::Task(const Function<void()>& func) : function(func) {
}
