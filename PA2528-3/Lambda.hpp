#ifndef __LAMBDA__
#define __LAMBDA__


template<typename T> class Lambda {
};

template<typename LambdaT, typename Return, typename... Params>
class LambdaHelper : public Lambda<Return(Params...)> {
    private:
        LambdaHelper(const LambdaT& lambda);
        virtual Return operator()(Params... args) override;

    private:
        friend Lambda<Return(Params...)>;
        LambdaT mLambda;
};

template<typename Return, typename... Params>
class Lambda<Return(Params...)> {
    public:
        template<typename LambdaT>
        static Lambda<Return(Params...)>* make(const LambdaT& lambda);
        virtual Return operator()(Params... args) = 0;

        virtual ~Lambda();
};

#include "Lambda.inl"

#endif // __LAMBDA__



