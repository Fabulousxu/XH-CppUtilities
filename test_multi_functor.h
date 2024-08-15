namespace xh{


template<class ...Functors>
struct multi_functor:public Functors...{
    using Functors::operator()...;
    constexpr multi_functor(Functors &&... functors) : Functors(functors)...{}
};
}