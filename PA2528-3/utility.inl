template<typename T>
constexpr bool isPow2(T t) {
    return t && !(t & (t - 1));
}
