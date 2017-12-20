template<typename T>
Promise<T> ThreadPool::promise(const Function<T()>& function) {
    SharedPtr<SharedPtr<T>> promise(new SharedPtr<T>());
    SharedPtr<bool> isReady(new bool());
    *isReady = false;
    Semaphore semaphore;

    launch([promise, isReady, semaphore, function]() {
		*promise = new T(function());
        *isReady = true; 
        semaphore.signal();
    });

    return Promise<T>(promise, isReady, semaphore);
}
