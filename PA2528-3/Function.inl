template<typename Return, typename... Params>
Function<Return(Params...)>::Function(Return (*function)(Params...))
: mCaller(&Function<Return(Params...)>::callFunction) {
    mFunction = function;
}

template<typename Return, typename... Params>
template<typename LambdaT>
Function<Return(Params...)>::Function(const LambdaT& lambda)
: mCaller(&Function<Return(Params...)>::callLambda) {
    mLambda = Lambda<Return(Params...)>::make(lambda);
}


template<typename Return, typename... Params>
Return Function<Return(Params...)>::operator()(Params... args) const {
    return (this->*mCaller)(args...);
}

template<typename Return, typename... Params>
Return Function<Return(Params...)>::callLambda(Params... args) const {
    return mLambda->operator()(args...);
}

template<typename Return, typename... Params>
Return Function<Return(Params...)>::callFunction(Params... args) const {
    return mFunction(args...);
}
