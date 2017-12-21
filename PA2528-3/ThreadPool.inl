template<typename T>
Promise<T> ThreadPool::promise(const Function<T()>& function) {
	Promise<T> promise;
    launch([promise]() {
		promise.fulfill(function());
    })
	return promise;
}
