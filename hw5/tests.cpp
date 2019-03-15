#ifndef TESTS
#define TESTS

#include <set>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <ctime>
#include "persistent_set.h"
#include "linked_ptr.h"
#include "shared_ptr.h"


const int N = 4000, TIMES = 5;

static std::vector<int> v;

static std::vector<int> gen_data() {
    std::vector<int> v;
    for (int i = 0; i < N; i++) {
        v.push_back(rand());
    }
    return v;
}

template<template<typename> class T>
void test1() {
    fprintf(stderr, "test1 : insert & erase -- random order: ");
    persistent_set<int, T> myset;

    std::random_shuffle(v.begin(), v.end());
    for (int x : v) {
        myset.insert(x);
    }

    std::random_shuffle(v.begin(), v.end());
    for (int x : v) {
        myset.erase(myset.find(x));
    }

    for (int x : v) {
        assert(myset.find(x) == myset.end());
    }

    fprintf(stderr, "ok\n");
}

template<template<typename> class T>
void test2() {
    fprintf(stderr, "test2 : insert & erase -- normal order: ");
    persistent_set<int, T> myset;

    std::sort(v.begin(), v.end());
    for (int x : v) {
        myset.insert(x);
    }

    for (int i = 0; i < N; i++) {
        assert(*myset.begin() == i);
        myset.erase(myset.begin());
    }

    for (int x : v) {
        assert(myset.find(x) == myset.end());
    }

    fprintf(stderr, "ok\n");
}

template<template<typename> class T>
void test3() {
    fprintf(stderr, "test3 : insert & erase -- reverse order: ");
    persistent_set<int, T> myset;

    std::sort(v.rbegin(), v.rend());
    for (int x : v) {
        myset.insert(x);
    }

    for (int i = N - 1; i >= 0; i--) {
        assert(*--myset.end() == i);
        myset.erase(--myset.end());
    }

    for (int x : v) {
        assert(myset.find(x) == myset.end());
    }

    fprintf(stderr, "ok\n");
}

template<template<typename> class T>
void test4() {
    fprintf(stderr, "test4 : iterate -- normal order: ");
    persistent_set<int, T> myset;

    std::random_shuffle(v.begin(), v.end());
    for (int x : v) {
        myset.insert(x);
    }

    std::vector<int> elems;
    for (int x : myset) {
        elems.push_back(x);
    }
    sort(v.begin(), v.end());
    assert(v == elems);

    fprintf(stderr, "ok\n");
}

template<template<typename> class T>
void test5() {
    fprintf(stderr, "test5 : iterate -- reverse order: ");
    persistent_set<int, T> myset;

    std::random_shuffle(v.begin(), v.end());
    for (int x : v) {
        myset.insert(x);
    }

    std::vector<int> elems;
    for (auto it = myset.end(); it != myset.begin();) {
        --it;
        elems.push_back(*it);
    }
    sort(v.rbegin(), v.rend());
    assert(v == elems);

    fprintf(stderr, "ok\n");
}

template<template<typename> class T>
void test6() {
    fprintf(stderr, "test6 : random insert, erase & iterate: ");
    persistent_set<int, T> myset;
    std::set<int> set;

    std::vector<int> data = gen_data();
    for (int x : data) {
        myset.insert(x);
    }

    std::vector<int> ans;
    for (int x : data) {
        if (x % 2 == 0) {
            myset.erase(myset.find(x));
        } else {
            ans.push_back(x);
        }
    }

    std::vector<int> elems;
    for (int x: myset) {
        elems.push_back(x);
    }


    fprintf(stderr, "ok\n");
}

struct no_def_constructor {
    no_def_constructor() = delete;
    no_def_constructor(int) {}
};

template<template<typename> class T>
void test7() {
    fprintf(stderr, "test7 : no default constructor: ");
    persistent_set<no_def_constructor, T> myset;

    fprintf(stderr, "ok\n");
}

template<template<typename> class T>
void test8() {
    fprintf(stderr, "test8 : persistency: ");
    std::vector<persistent_set<int, T>> myset_v(1);
    std::vector<std::set<int>> stdset_v(1);
    for (int i = 0; i < N; i++) {
        if (rand() % 16) {
            myset_v.back().insert(i);
            stdset_v.back().insert(i);
        } else {
            myset_v.push_back(persistent_set<int, T>(myset_v.back()));
            stdset_v.push_back(std::set<int>(stdset_v.back()));
        }
    }
    for (size_t i = 0; i < myset_v.size(); i++) {
        std::vector<int> my_elems, std_elems;
        for (int x: myset_v[i]) {
            my_elems.push_back(x);
        }
        for (int x: stdset_v[i]) {
            std_elems.push_back(x);
        }
        assert(my_elems == std_elems);
    }

    fprintf(stderr, "ok\n");
}


template<template<typename> class T>
long long test9() {
    fprintf(stderr, "test9 : performance: \n");

    std::vector<persistent_set<int, T>> myset_v(1);

    long long s = 0;
    for (int i = 0; i < TIMES; i++) {
        fprintf(stderr, "    warmup, iteration %d\n", i + 1);
        myset_v = std::vector<persistent_set<int, T>>(1);

        for (int i = 0; i < N; i++) {
            if (rand() % 8) {
                myset_v.back().insert(i);
            } else {
                myset_v.push_back(persistent_set<int, T>(myset_v.back()));
            }
        }
        s = 0;
        for (size_t i = 0; i < myset_v.size(); i++) {
            for (int x: myset_v[i]) {
                s += x;
            }
        }
        for (size_t i = 0;i < myset_v.size(); i++) {
            myset_v[i].erase(--myset_v[i].end());
        }
    }

    double ans = 0.0;
    for (int i = 0; i < TIMES; i++) {
        fprintf(stderr, "    testing, iteration %d: ", i + 1);
        myset_v = std::vector<persistent_set<int, T>>(1);
        std::clock_t start = std::clock();

        for (int i = 0; i < N; i++) {
            if (rand() % 8) {
                myset_v.back().insert(i);
            } else {
                myset_v.push_back(persistent_set<int, T>(myset_v.back()));
            }
        }
        s = 0;
        for (size_t i = 0; i < myset_v.size(); i++) {
            for (int x: myset_v[i]) {
                s += x;
            }
        }
        for (size_t i = 0;i < myset_v.size(); i++) {
            myset_v[i].erase(--myset_v[i].end());
        }
        double tmp = (clock() - start) / ((double) CLOCKS_PER_SEC / 1000.0);
        ans += tmp;
        fprintf(stderr, "%.2lf ms\n", tmp);
    }
    fprintf(stderr, "    result: %.2lf ms\n", ans / (double) TIMES);

    return s;
}



static void prepare() {
    v.clear();
    for (int i = 0; i < N; i++) {
        v.push_back(i);
    }
}

template<template<typename> class T>
void run() {
    prepare();
    test1<T>();
    test2<T>();
    test3<T>();
    test4<T>();
    test5<T>();
    test6<T>();
    test7<T>();
    test8<T>();
    fprintf(stderr, "\n");
}


template<template<typename> class T>
void run_perf() {
    test9<T>();
    fprintf(stderr, "\n");
}

#endif // TESTS
