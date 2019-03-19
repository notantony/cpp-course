#include <set>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <tuple>
#include <functional>
#include <memory>
#include "bind.h"


template<typename T>
void check() {
    std::cout <<  __PRETTY_FUNCTION__  << std::endl;
}

template<typename T>
void check(T) {
    std::cout <<  __PRETTY_FUNCTION__  << std::endl;
}


template<int ... XS>
struct IntList;

template<>
struct IntList<> {
};

template<int X, int ... XT>
struct IntList<X, XT...> {
    static const int Head = X;
    using Tail = IntList<XT...>;
};

template<typename IL>
struct Length {
    static const int value = 1 + Length<typename IL::Tail>::value;
};

template<>
struct Length<IntList<>> {
    static const int value = 0;
};

template<int X, typename XT>
struct IntCons;

template<int X, int ... XT>
struct IntCons<X, IntList<XT...>> {
    using type = IntList<X, XT...>;
};

template<int N, int K = N>
struct Generate {
    using type = typename IntCons<K - N, typename Generate<N - 1, K>::type>::type;
};

template<int N>
struct Generate<0, N> {
    using type = IntList<>;
};


//template<typename T, typename ... Args>
//T apply(T f, std::tuple<Args...> args) {
//    return f(std::get<Generate<sizeof...(Args)>::type>(args) ...);
//}
/*
template <typename F, typename TUP,
          int ... INDICES>
static void tuple_call_(F f, TUP tup,
                       IntList<INDICES...>)
{
    f(std::get<INDICES>(tup) ...);
}

template <typename F, typename ... Ts>
static void tuple_call(F f, std::tuple<Ts...> tup)
{
    tuple_call_(f, tup,
                Generate<sizeof...(Ts)>::type);
}
 */
//template <typename F, typename TP, int ... Ids>
//auto sss(F f, TP args, IntList<Ids ...>) -> decltype(f(std::get<Ids>(args) ...)) {
//    return f(std::get<Ids>(args) ...);
//}

//template <typename F, typename ... Args>
//auto apply(F f, std::tuple<Args...> args) -> decltype(sss(f, args, Generate<sizeof...(Args)>::type)) {
//    return sss(f, args, Generate<sizeof...(Args)>::type);
//}


//template<int ...> struct seq {};

//template<int N, int ...S> struct gens : gens<N-1, N-1, S...> {};

//template<int ...S> struct gens<0, S...>{ typedef seq<S...> type; };


//template <typename ...Args>
//struct save_it_for_later
//{
//  std::tuple<Args...> params;
//  double (*func)(Args...);

//  double delayed_dispatch()
//  {
//    return callFunc(typename gens<sizeof...(Args)>::type());
//  }

//  template<int ...S>
//  double callFunc(seq<S...>)
//  {
//    return func(std::get<S>(params) ...);
//  }
//};


//template <typename T, T ... values>
//struct qwe{

//};

void f(int a, int b, int c)
{
    std::cout << a << b << c << std::endl;
}

int f2(int a, int b)
{
    std::cout << a << b << std::endl;
    return 2;
}


int f1(int a)
{
    std::cout << a << std::endl;
    return 1;
}

int f0()
{
    std::cout << "Zero" << std::endl;
    return 0;
}

int g(int a, int b)
{
    return a + b;
}


int uni_f(std::unique_ptr<int> &&a) {
    std::cout << a.get();
    return 1;
}


int main()
{

//    auto f = [](int x, double y, double z) { return x + y + z; };
//    auto t = std::make_tuple(30, 5.0, 1.6);  // std::tuple<int, double, double>
    //auto res = apply(f, t);


//    auto qwe = std::make_tuple(std::get<2, 2>(t));
//    std::cout << std::get<1>(t);

//        check<std::get<1, 2, 3>>();
//    check<Generate<5>::type>();
//    check<IntCons<4, IntList<1, 3, 4>>::type>();
    using std::cout;

    cout << any_eq<10, integer_sequence<int, 1, 2, 3, 4, 6, 4>>::value<< "\n";

    G<placeholder<7>> qwe((placeholder<7>()));
    cout << qwe(1, 3, 4, 5, 7, 8, 0.5, 0) << "\n";

    check<make_integer_sequence<int, 5>::type>();

    check<get_ints<placeholder<3>, placeholder<3>,
                        placeholder<7>,
                        placeholder<4>,
                        placeholder<2>,
                        int,
                        double,
                        double,
                        bind_t<void(int, int),
                                int,
                                placeholder<6>,
                                bind_t<placeholder<6>,
                                        placeholder<4>,
                                        placeholder<4>>,
                                int,
                                int,
                                placeholder<4>>>::type>();

    check<wrap<placeholder<4>, integer_sequence<int, 1, 4, 4, 5, 6, 5>>::type>();
    check<wrap<placeholder<7>, integer_sequence<int, 1, 3, 4, 5, 6, 5>>::type>();
    bind(&f, 1, 2, placeholder<1>())(3);
    auto t = bind(&f, _1, placeholder<2>(), _3, _5, 432, 32, _5, placeholder<5>()); // (4, 6, 5);
    check(t.gs);
    bind(&f, _1, _1, _2)(7, 8);
    auto tm1 = bind(&f, _1, _1, _2);
    check(tm1.gs);

    auto tmp = bind(&f2, _4, bind(&g, _4, _4));
    check(&tmp);
    check(tmp.gs);
    check(&std::get<0>(tmp.gs));
    check(&std::get<1>(tmp.gs).fun.gs);
    //check<::ints>();

//    check<typename single_wrap<bind_t<int (*)(int, int), placeholder<4>, placeholder<4> >,
//            integer_sequence<int, 4, 4, 4> >::type>();

    auto tmp0 = bind(&f1, bind(&f1, 4));
    check(tmp0.gs);

    bind(&f1, bind(&f1, _1))(1, 3);
    bind(&f, _1, bind(&g, _1, 5), _2)(2, 3);
    bind(&f, _1, bind(&g, _1, 5), _2)(2, 3);

    std::unique_ptr<int> uni3(new int(3));
    call_once_bind(&uni_f, std::move(uni3));

//    cout << any_eq<1, placeholder<3>, placeholder<3>,
//            placeholder<7>,
//            placeholder<4>,
//            placeholder<2>,
//            int,
//            double,
//            double,
//            bind_t<void(int, int),
//                    int,
//                    int,
//                    int,
//                    placeholder<4>>
//            >::value << "\n";
    //placeholder<5> x;

//    check<
//        wrap<
//            bind_t<
//                    void(),
//                    bind_t<
//                            void(int, int),
//                            placeholder<1>,
//                            double
//                    >,
//                    placeholder<1>,
//                    int
//            >,
//            integer_sequence<int, 1, 2, 3, 4, 1>
//        >::type
//    >();


//    check<purge<const placeholder<5>&>::type>();
//    check<purge<placeholder<5>&&>::type>();
//    check<purge<placeholder<5>&>::type>();
//    check<purge<placeholder<5>>::type>();

//    check<
//            purge<
//                    bind_t<
//                            placeholder<3>,
//                            placeholder<3>,
//                            placeholder<7>,
//                            placeholder<4>,
//                            placeholder<2>,
//                            int,
//                            double,
//                            double,
//                            bind_t<void(int, int),
//                                    int,
//                                    placeholder<6>,
//                                    bind_t<placeholder<6>,
//                                            const placeholder<4>&,
//                                            const placeholder<4>&
//                                    >,
//                                    int,
//                                    int,
//                                    placeholder<4>
//                            >
//                     >
//            >::type
//    >();

    //std::tuple<G<bind_t<int (*)(int), placeholder<1> > > > (bind(&f, _1));
            //bind_t<int (*)(int), const placeholder<1>&>

    //std::tuple<G<bind_t<int (*)(int), placeholder<1> > > > a(bind_t<int (*)(int), const placeholder<1>&>);

    //bind_t<int (*)(int), const placeholder<1>&> q = bind(&f1, _1);
    //G<bind_t<int (*)(int), placeholder<1> > > b((q));


    return 0;
}
