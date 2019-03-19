#ifndef BIND_H
#define BIND_H

#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

using std::enable_if;
using std::forward;
using std::remove_reference;

template <typename T, T ... values>
struct integer_sequence{};

template <typename T, typename Seq, T arg>
struct append;

template <typename T, T ... values, T arg>
struct append<T, integer_sequence<T, values...>, arg>
{
    using type = integer_sequence<T, values..., arg>;
};

template <typename T, typename X, typename Y>
struct merge;

template <typename T, T ... As, T ... Bs>
struct merge<T, integer_sequence<T, As ...>, integer_sequence<T, Bs ...> >{
    using type = integer_sequence<T, As ..., Bs ...>;
};

template <typename T, T N, typename Q = void>
struct make_integer_sequence
{
    using type = typename append<T, typename make_integer_sequence<T, N - 1>::type, N - 1>::type;
};

template <typename T, T N>
struct make_integer_sequence<T, N, typename std::enable_if<N == 0>::type>
{
    using type = integer_sequence<T>;
};

template <int N>
struct placeholder {};

template <int N>
struct copy_placeholder {};

template <typename F, typename ... As>
struct bind_t;

constexpr placeholder<1> _1;
constexpr placeholder<2> _2;
constexpr placeholder<3> _3;
constexpr placeholder<4> _4;
constexpr placeholder<5> _5;
constexpr placeholder<6> _6;
constexpr placeholder<7> _7;
constexpr placeholder<8> _8;
constexpr placeholder<9> _9;

template<int X, typename Seq>
struct any_eq;

template<int X, int ... As>
struct any_eq<X, integer_sequence<int, X, As ...> > {
    const static int value = 1 + any_eq<X, integer_sequence<int, As ...> >::value;
};

template<int X, int A, int ... As>
struct any_eq<X, integer_sequence<int, A, As ...>> {
    const static int value = any_eq<X, integer_sequence<int, As ...> >::value;
};

template<int X>
struct any_eq<X, integer_sequence<int>> {
    const static bool value = 0;
};

template<typename ... As>
struct get_ints;

template<typename T, typename ... As>
struct get_ints<T, As ...> {
    using type = typename get_ints<As ...>::type;
};

template<>
struct get_ints<> {
    using type = integer_sequence<int>;
};

template<typename F, typename ... Bs, typename ... As>
struct get_ints<bind_t<F, Bs ...>, As ...> {
    using type = typename merge<int,
            typename get_ints<Bs ...>::type,
            typename get_ints<As ...>::type
            >::type;
};

template<int N, typename ... As>
struct get_ints<placeholder<N>, As ...> {
    using type = typename append<int, typename get_ints<As ...>::type, N>::type;
};

template<typename T, typename Seq, typename I = void>
struct wrap {
    using type = T;
};

template<int N, int ... As>
struct wrap<placeholder<N>, integer_sequence<int, As ...>,
        typename std::enable_if<any_eq<N, integer_sequence<int, As ...> >::value >= 2>::type > {
    using type = copy_placeholder<N>;
};

template<typename F, typename ... Bs, int ... As>
struct wrap<bind_t<F, Bs ...>, integer_sequence<int, As ...> > {
    using type = bind_t<F, typename wrap<Bs, integer_sequence<int, As ...> >::type ...>;
};


template<typename T>
struct purge {
    using type = T;
};

template<int N>
struct purge<placeholder<N>&&> {
    using type = placeholder<N>;
};

template<int N>
struct purge<placeholder<N>&> {
    using type = placeholder<N>;
};

template<int N>
struct purge<const placeholder<N>&> {
    using type = placeholder<N>;
};


template<typename T>
struct purge_bind {
    using type = T;
};

template<typename F, typename ... As>
struct purge_bind<bind_t<F, As ...>> {
    using type = bind_t<F, As ...>;
};

template<typename F, typename ... As>
struct purge_bind<bind_t<F, As ...>&&> {
    using type = bind_t<F, As ...>;
};

template<typename F, typename ... As>
struct purge_bind<bind_t<F, As ...>&> {
    using type = bind_t<F, As ...>;
};

template<typename F, typename ... As>
struct purge_bind<const bind_t<F, As ...>&> {
    using type = bind_t<F, As ...>;
};


template<typename A>
struct G
{
    template<typename _A>
    G(_A &&a)
        : a(std::forward<_A>(a))
    {}

    template <typename ... Bs>
    A operator()(Bs&& ...)
    {
        return static_cast<A>(a);
    }

    typename std::remove_reference<A>::type a;
};

template<>
struct G<placeholder<1> >
{
    G(placeholder<1>)
    {}

    template <typename B1, typename ... Bs>
    B1 operator()(B1&& b1, Bs&& ...) const
    {
        return std::forward<B1>(b1);
    }
};

template<>
struct G<copy_placeholder<1> >
{
    G(copy_placeholder<1>)
    {}
    G(placeholder<1>)
    {}

    template <typename B1, typename ... Bs>
    B1 operator()(B1&& b1, Bs&& ...) const
    {
        return b1;
    }
};

template<int N>
struct G<placeholder<N> >
{
    G(placeholder<N>)
    {}

    template <typename B, typename ... Bs>
    decltype(auto) operator()(B&&, Bs&& ... bs) const
    {
        G<placeholder<N - 1>> next((placeholder<N - 1>()));
        return next(std::forward<Bs>(bs)...);
    }
};

template<int N>
struct G<copy_placeholder<N> >
{
    G(placeholder<N>)
    {}
    G(copy_placeholder<N>)
    {}

    template <typename B, typename ... Bs>
    decltype(auto) operator()(B&&, Bs&& ... bs) const
    {
        G<copy_placeholder<N - 1>> next((copy_placeholder<N - 1>()));
        return next(std::forward(bs)...);
    }
};

template<typename F, typename ... As>
struct G<bind_t<F, As...> >
{
    template<typename _F, typename ... _As>
    G(bind_t<_F, _As...> &&fun)
        : fun(std::move(fun))
    {}

    template<typename _F, typename ... _As>
    G(bind_t<F, As...> &fun)
        : fun(fun)
    {}

    template<typename _F, typename ... _As>
    G(const bind_t<F, As...> &fun)
        : fun(fun)
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs&& ... bs)
    {
        return fun(std::forward<Bs>(bs)...);
    }

    bind_t<F, As...> fun;
};

template<typename F, typename ... As>
struct bind_t
{

    template<typename _F, typename ... _As>
    bind_t(_F&& f, _As&& ... as)
        : f(std::forward<_F>(f)),
          gs(std::forward<_As>(as)...)
    {}

    template <typename ... Bs>
    decltype(auto) operator()(Bs&& ... bs)
    {
        return call(typename make_integer_sequence<int, sizeof...(As)>::type(),
                    std::forward<Bs>(bs)...);
    }

//private:
    template <int... ks, typename ... Bs>
    decltype(auto) call(integer_sequence<int, ks...>, Bs&& ... bs)
    {
        return f(std::get<ks>(gs)(std::forward<Bs>(bs)...)...);
    }


    using ints = typename get_ints<typename purge_bind<typename purge<As>::type>::type ...>::type;

    F f;
    std::tuple<G<typename purge_bind<typename wrap<As, ints>::type>::type>...> gs;
};

template <typename F, typename ... As>
decltype(auto) bind(F&& f, As&& ... as)
{
    return bind_t<typename std::decay<F>::type,
            typename purge<typename std::decay<As>::type&>::type ...>(std::forward<F>(f), std::forward<As>(as)...);
}

template <typename F, typename ... As>
decltype(auto) call_once_bind(F&& f, As&& ... as)
{
    return bind_t<typename std::decay<F>::type,
            typename purge<typename std::decay<As>::type&&>::type ...>(std::forward<F>(f), std::forward<As>(as)...);
}

#endif // BIND_H

//::tuple< G<placeholder<4> >,  G<bind_t<int (*)(int, int), placeholder<4>, placeholder<4> > > >
//::tuple(const placeholder<4>&, bind_t<int (*)(int, int), placeholder<4>, placeholder<4> >)’

//‘bind_t<int (*)(std::unique_ptr<int>&&), std::unique_ptr<int, std::default_delete<int> >&&>’
//‘int (*)(std::unique_ptr<int>&&)’

//bind_t<int (*)(std::unique_ptr<int>&&, int), std::unique_ptr<int, std::default_delete<int> >&&, placeholder<1> >’
//‘int (*)(std::unique_ptr<int>&&, int)
