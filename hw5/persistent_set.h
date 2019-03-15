#ifndef PERSISTENT_SET_H
#define PERSISTENT_SET_H
#include <iterator>
#include <memory>
#include "shared_ptr.h"
#include "linked_ptr.h"

using std::swap;

template<typename T, template<typename> class PTR = linked_ptr>
class persistent_set {
public:
    class iterator;
    struct node;
    typedef T value_type;

    persistent_set();
    persistent_set(const persistent_set&) noexcept;
    persistent_set(persistent_set&& other) noexcept;

    persistent_set& operator=(const persistent_set& other) noexcept;
    persistent_set& operator=(persistent_set&& other) noexcept;
    void swap(persistent_set& other) noexcept(noexcept(swap(other.root, other.root)));

    ~persistent_set() noexcept = default;

    iterator find(value_type) const noexcept;
    std::pair<iterator, bool> insert(value_type);

    void erase(iterator);

    iterator begin() const noexcept;
    iterator end() const noexcept;

    friend void swap(persistent_set &a, persistent_set &b) noexcept(noexcept(a.swap(a.root))) {
        a.swap(b);
    }

private:
    persistent_set(const node &a) : root(PTR<node>(new node())) {
        root.get()->l = PTR<node>(new node(a));
    }
    PTR<node> root;
};


template<typename T, template<typename> class PTR>
struct persistent_set<T, PTR>::node {
    PTR<T> a;
    PTR<node> l, r;

    node() noexcept = default;
    node(const node &other) noexcept(noexcept(PTR<T>(a)) && noexcept(PTR<node>(l))) : a(other.a), l(other.l), r(other.r) {}
    node(const T &a) : a(PTR<T>(new T(a))) {}
};


template<typename T, template<typename> class PTR>
class persistent_set<T, PTR>::iterator {
    friend class persistent_set<T, PTR>;
public:
    typedef std::bidirectional_iterator_tag iterator_category;
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;

    value_type const& operator*() const noexcept {
        return *a->a.get();
    }

    iterator& operator++() noexcept {
        next();
        return *this;
    }

    iterator operator++(int) noexcept {
        iterator tmp = *this;
        next();
        return tmp;
    }

    iterator& operator--() noexcept {
        prev();
        return *this;
    }

    iterator operator--(int) noexcept {
        iterator tmp = *this;
        prev();
        return tmp;
    }

    friend bool operator==(iterator a, iterator b) noexcept {
        return a.a == b.a;
    }

    friend bool operator!=(iterator a, iterator b) noexcept {
        return a.a != b.a;
    }
private:
    node *a, *root;
    iterator() = delete;
    iterator(node *a, node *root) : a(a), root(root) {}

    void next() {
        value_type *x = a->a.get();
        if (a->r.get() != nullptr) {
            a = a->r.get();
            while (a->l.get() != nullptr) {
                a = a->l.get();
            }
            return;
        }
        a = root;
        node* cur = root->l.get();
        while (*x != *cur->a.get()) {
            if (*x < *cur->a.get()) {
                a = cur;
                cur = cur->l.get();
            } else {
                cur = cur->r.get();
            }
        }
    }

    void prev() {
        value_type *x = a->a.get();
        if (a->l.get() != nullptr) {
            a = a->l.get();
            while (a->r.get() != nullptr) {
                a = a->r.get();
            }
            return;
        }
        a = root;
        node* cur = root->l.get();
        //assert(x != nullptr);
        if (x == nullptr) {
            return;
        }
        while (*x != *cur->a.get()) {
            if (*x < *cur->a.get()) {
                cur = cur->l.get();
            } else {
                a = cur;
                cur = cur->r.get();
            }
        }
    }
};


template<typename T, template<typename> class PTR>
persistent_set<T, PTR>::persistent_set() : root(PTR<node>(new node())) {}

template<typename T, template<typename> class PTR>
persistent_set<T, PTR>::persistent_set(const persistent_set& other) noexcept : root(other.root) {}

template<typename T, template<typename> class PTR>
persistent_set<T, PTR>::persistent_set(persistent_set&& other) noexcept {
    swap(other);
}

template<typename T, template<typename> class PTR>
persistent_set<T, PTR> &persistent_set<T, PTR>::operator=(const persistent_set& other) noexcept {
    root = other.root;
    return *this;
}

template<typename T, template<typename> class PTR>
void persistent_set<T, PTR>::swap(persistent_set& other) noexcept(noexcept(swap(other.root, other.root))) {
    std::swap(root, other.root);
}

template<typename T, template<typename> class PTR>
persistent_set<T, PTR> &persistent_set<T, PTR>::operator=(persistent_set &&other) noexcept {
    swap(*this, other);
    return *this;
}

template<typename T, template<typename> class PTR>
typename persistent_set<T, PTR>::iterator persistent_set<T, PTR>::find(value_type x) const noexcept {
    node *cur = root.get()->l.get();

    while (cur != nullptr) {
        if (x < *cur->a.get()) {
            cur = cur->l.get();
        } else if (x > *cur->a.get()) {
            cur = cur->r.get();
        } else {
            return iterator(cur, root.get());
        }
    }

    return iterator(root.get(), root.get());
}

template<typename T, template<typename> class PTR>
std::pair<typename persistent_set<T, PTR>::iterator, bool> persistent_set<T, PTR>::insert(value_type x) {
    root = PTR<node>(new node(*root.get()));
    PTR<node> *edge = &root.get()->l;
    node *cur;

    while ((cur = edge->get()) != nullptr) {
        *edge = PTR<node>(new node(*cur));
        cur = edge->get();
        if (x < *cur->a.get()) {
            edge = &cur->l;
        } else if (x > *cur->a.get()) {
            edge = &cur->r;
        } else {
            return std::make_pair(iterator(cur, root.get()), false);
        }
    }

    *edge = PTR<node>(new node(x));
    return std::make_pair(iterator(edge->get(), root.get()), true);
}

template<typename T, template<typename> class PTR>
void persistent_set<T, PTR>::erase(iterator it) {
    value_type *x = it.a->a.get();

    root = PTR<node>(new node(*root.get()));
    PTR<node> *edge = &root.get()->l;
    node *cur;

    while ((cur = edge->get()) != nullptr) {
        *edge = PTR<node>(new node(*cur));
        cur = edge->get();
        if (*x < *cur->a.get()) {
            edge = &cur->l;
        } else if (*x > *cur->a.get()) {
            edge = &cur->r;
        } else {
            if (cur->l.get() == nullptr && cur->r.get() == nullptr) {
                edge->reset();
            } else if (cur->l.get() == nullptr) {
                *edge = PTR<node>(new node(*cur->r.get()));
            } else if (cur->r.get() == nullptr) {
                *edge = PTR<node>(new node(*cur->l.get()));
            } else {
                persistent_set tmp = persistent_set(*cur->r.get());
                iterator least = tmp.begin();
                cur->a = least.a->a;
                tmp.erase(least);
                cur->r = tmp.root.get()->l;
            }
            return;
        }
    }
    //no such element
}


template<typename T, template<typename> class PTR>
typename persistent_set<T, PTR>::iterator persistent_set<T, PTR>::begin() const noexcept {
    node *cur = root.get();

    while (cur->l.get() != nullptr) {
        cur = cur->l.get();
    }

    return iterator(cur, root.get());
}

template<typename T, template<typename> class PTR>
typename persistent_set<T, PTR>::iterator persistent_set<T, PTR>::end() const noexcept {
    return iterator(root.get(), root.get());
}


#endif
