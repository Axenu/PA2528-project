template<typename LambdaT, typename Return, typename... Params>
LambdaHelper<LambdaT, Return, Params...>::LambdaHelper(const LambdaT& lambda) : mLambda(lambda) {
}

template<typename LambdaT, typename Return, typename... Params>
Return LambdaHelper<LambdaT, Return, Params...>::operator()(Params... args) {
    return mLambda(args...);
}


template<typename Return, typename... Params>
template<typename LambdaT>
Lambda<Return(Params...)>* Lambda<Return(Params...)>::make(const LambdaT& lambda) {
    return new LambdaHelper<LambdaT, Return, Params...>(lambda);
}

template<typename Return, typename... Params>
Lambda<Return(Params...)>::~Lambda() {

}
