#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <cstddef>
#include <algorithm>

template<typename T>
class shared_ptr {
private:
    struct data;

    data *dt = nullptr;
public:
    shared_ptr() noexcept = default;
    shared_ptr(T *a);
    shared_ptr(const shared_ptr<T> &) noexcept;
    shared_ptr(shared_ptr<T> &&) noexcept;

    T *get() const;
    void reset() noexcept;
    void swap(shared_ptr<T> &other) noexcept;
    shared_ptr<T> &operator=(const shared_ptr<T> &other) noexcept;
    shared_ptr<T> &operator=(shared_ptr<T> &&other) noexcept;

    ~shared_ptr() noexcept;
    friend void swap(shared_ptr<T> &a, shared_ptr<T> &b) noexcept {
        a.swap(b);
    }
};

template<typename T>
struct shared_ptr<T>::data {
    friend class shared_ptr<T>;
    T *obj;
    size_t cnt = 1; 

    data() = delete;
    data(T *obj) noexcept : obj(obj) {}

    ~data() noexcept {
        delete(obj);
    }

};

template<typename T>
shared_ptr<T>::shared_ptr(T *obj) {
    if (obj == nullptr) {
        return;
    }
    dt = new data(obj);
}

template<typename T>
shared_ptr<T>::shared_ptr(const shared_ptr<T> &other) noexcept : dt(other.dt) {
    if (dt == nullptr) {
        return;
    }
    dt->cnt++;
}

template<typename T>
shared_ptr<T>::shared_ptr(shared_ptr<T> &&other) noexcept {
    swap(other);
}

template<typename T>
T *shared_ptr<T>::get() const {
    if (dt == nullptr) {
        return nullptr;
    }
    return dt->obj;
}

template<typename T>
void shared_ptr<T>::reset() noexcept {
    if (dt == nullptr) {
        return;
    }
    if (!(--dt->cnt)) {
        delete dt;
    }
    dt = nullptr;
}

template<typename T>
void shared_ptr<T>::swap(shared_ptr<T> &other) noexcept {
    std::swap(dt, other.dt);
}

template<typename T>
shared_ptr<T> &shared_ptr<T>::operator=(const shared_ptr<T> &other) noexcept {
    shared_ptr<T> tmp = other;
    swap(tmp);
    return *this;
}

template<typename T>
shared_ptr<T> &shared_ptr<T>::operator=(shared_ptr<T> &&other) noexcept {
    swap(other);
    return *this;
}

template<typename T>
shared_ptr<T>::~shared_ptr() noexcept {
    reset();
}

#endif // SHARED_PTR_H
