#ifndef LINKED_PTR_H
#define LINKED_PTR_H

#include <algorithm>

template<typename T>
class linked_ptr {
private:
    T *data = nullptr;
    mutable const linked_ptr<T> *l = nullptr, *r = nullptr;

public:
    linked_ptr() noexcept = default;
    linked_ptr(T *a) noexcept;
    linked_ptr(const linked_ptr<T> &) noexcept;
    linked_ptr(linked_ptr<T> &&) noexcept;

    T *get() const noexcept;
    void reset() noexcept;
    linked_ptr<T> &operator=(const linked_ptr<T> &) noexcept;
    linked_ptr<T> &operator=(linked_ptr<T> &&) noexcept;
    void swap(linked_ptr<T> &) noexcept;

    ~linked_ptr();

    friend void swap(linked_ptr<T> &a, linked_ptr<T> &b) noexcept {
        a.swap(b);
    }
};

template<typename T>
linked_ptr<T>::linked_ptr(T *a) noexcept : data(a) {
    if (a == nullptr) {
        return;
    }
    l = r = this;
}

template<typename T>
linked_ptr<T>::linked_ptr(const linked_ptr<T> &other) noexcept : data(other.data), l(other.l), r(&other) {
    if (other.data == nullptr) {
        return;
    }
    other.l->r = this;
    other.l = this;
}


template<typename T>
linked_ptr<T>::linked_ptr(linked_ptr<T> &&other) noexcept : data(other.data), l(other.l), r(other.r) {
    if (data != nullptr) {
        l->r = this;
        r->l = this;
    }
    //other.reset();
}

template<typename T>
T *linked_ptr<T>::get() const noexcept {
    return data;
}

template<typename T>
void linked_ptr<T>::reset() noexcept {
    if (data == nullptr) {
        return;
    }
    if (l == this) {
        delete data;
    } else {
        l->r = r;
        r->l = l;
    }
    l = r = nullptr;
    data = nullptr;
}

template<typename T>
void linked_ptr<T>::swap(linked_ptr<T> &tmp) noexcept {
    linked_ptr<T> const *new_l = tmp.l, *new_r = tmp.r;
    if (data != nullptr) {
        linked_ptr<T> const *cur_l = l, *cur_r = r;
        std::swap(cur_l->r, new_l->r);
        std::swap(cur_r->l, new_r->l);
    } else {
        new_l->r = this;
        new_r->l = this;
    }
    std::swap(data, tmp.data);
    std::swap(l, tmp.l);
    std::swap(r, tmp.r);
}

template<typename T>
linked_ptr<T> &linked_ptr<T>::operator=(const linked_ptr<T> &other) noexcept {
    if (other.data == nullptr) {
        reset();
        return *this;
    }
    linked_ptr<T> tmp = other;
    swap(tmp);
    return *this;
}

template<typename T>
linked_ptr<T> &linked_ptr<T>::operator=(linked_ptr<T> &&other) noexcept {
    if (other.data == nullptr) {
        reset();
        return *this;
    }
    swap(other);
    return *this;
}

template<typename T>
linked_ptr<T>::~linked_ptr() {
    reset();
}

#endif // LINKED_PTR_H

