#ifndef MYLIST_H
#define MYLIST_H

#include <stddef.h>
#include <memory>

using std::shared_ptr;
using std::weak_ptr;
using std::make_shared;

template<typename T>
class list {
private:
    struct node {
        friend class list;
        node *l = this, *r = this;
        T* value = nullptr;

        node();
        node(const T &);
        ~node();
    };
    mutable node head;

public:
    class const_iterator;
    class iterator {
    friend class list;
    public:
        typedef std::ptrdiff_t difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::bidirectional_iterator_tag iterator_category;

        iterator() = default;
        iterator(const iterator &);
        iterator &operator=(const const_iterator &);
        //iterator &operator=(const iterator &) = delete;

        bool operator==(const const_iterator &) const;
        bool operator!=(const const_iterator &) const;

        T &operator*();

        iterator &operator++();
        iterator operator++(int);
        iterator &operator--();
        iterator operator--(int);
    private:
        node *ptr = nullptr;

        iterator(node *);
	};

    class const_iterator {
        friend class list;
        public:
            typedef std::ptrdiff_t difference_type;
            typedef const T value_type;
            typedef const T* pointer;
            typedef const T& reference;
            typedef std::bidirectional_iterator_tag iterator_category;

            const_iterator() = default;
            const_iterator(const const_iterator &);
            const_iterator(const iterator &);
            bool operator==(const const_iterator &one) const;
            bool operator!=(const const_iterator &one) const;

            const T &operator*();

            const_iterator &operator++();
            const_iterator operator++(int);
            const_iterator &operator--();
            const_iterator operator--(int);
        private:
            node *ptr = nullptr;

            const_iterator(list<T>::node *);
        };

    typedef T value_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    template<typename TT>
    friend void swap(list<TT> &a, list<TT> &b) {
        std::swap(a.head.l, b.head.l);
        std::swap(a.head.r, b.head.r);
        std::swap(a.head, b.head);
    }

    list();
    list(const list<T> &);

    iterator insert(const const_iterator &, const T &);
    iterator erase(const const_iterator &);
    iterator erase(const_iterator, const const_iterator &);

    void splice(const const_iterator &, list<T> &);
    void splice(const const_iterator &, list<T> &, const const_iterator&);
    void splice(const const_iterator &, list<T> &, const const_iterator&, const const_iterator&);

    void clear();
    void operator=(const list<T> &);

    void push_back(const T &);
    void push_front(const T &);

    void pop_back();
    void pop_front();

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    size_t size() const;
    bool empty() const;

    iterator begin() const;
    iterator end() const;
    reverse_iterator rbegin() const;
    reverse_iterator rend() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    ~list();
};

template<typename T>
list<T>::node::node() : l(this), r(this) {}

template<typename T>
list<T>::node::node(const T &one) : l(this), r(this), value(new T(one)) {}

template<typename T>
list<T>::node::~node() {
    if (value != nullptr) {
        delete(value);
    }
}


template<typename T>
list<T>::list() {}
template<typename T>
list<T>::list(const list<T> &one) {
    for (auto it = one.begin(); it != one.end(); ++it) {
        push_back(*it);
    }
}

template<typename T>
typename list<T>::iterator list<T>::insert(const const_iterator &pos, const T &value) {
    node *next = pos.ptr;
    node *prev = next->l;
    prev->r = new node(value);
    next->l = prev->r;
    prev->r->l = prev;
    next->l->r = next;
    return iterator(prev->r);
}
template<typename T>
typename list<T>::iterator list<T>::erase(const const_iterator &pos) {
    node *next = pos.ptr->r;
    node *prev = pos.ptr->l;
    prev->r = next;
    next->l = prev;
    delete(pos.ptr);
    return iterator(next);
}
template<typename T>
typename list<T>::iterator list<T>::erase(const_iterator from, const const_iterator &to) {
    while(from != to) {
        from = erase(from);
    }
    begin().ptr;
    return iterator(from.ptr);
}

template<typename T>
void list<T>::splice(const const_iterator &pos, list<T> &one) {
    splice(pos, one, one.begin(), one.end());
}
template<typename T>
void list<T>::splice(const const_iterator &pos, list<T> &one, const const_iterator &at) {
    const_iterator tmp = at;
    splice(pos, one, at, ++tmp);
}
template<typename T>
void list<T>::splice(const const_iterator &pos, list<T> &, const const_iterator &from, const const_iterator &to) {
    if (from == to) {
        return;
    }
    node *next = pos.ptr;
    node *prev = pos.ptr->l;
    node *old_next = to.ptr;
    node *old_prev = from.ptr->l;
    node *r = to.ptr->l;
    node *l = from.ptr;

    old_prev->r = old_next;
    old_next->l = old_prev;
    prev->r = l;
    next->l = r;
    l->l = prev;
    r->r = next;
}

template<typename T>
void list<T>::clear() {
    erase(begin(), end());
}
template<typename T>
void list<T>::operator=(const list<T> &one) {
    list<T> tmp(one);
    swap(*this, tmp);
}

template<typename T>
void list<T>::push_back(const T &one) {
    insert(end(), one);
}
template<typename T>
void list<T>::push_front(const T &one) {
    insert(begin(), one);
}

template<typename T>
void list<T>::pop_back() {
    auto tmp = --end();
    erase(tmp, end());
}
template<typename T>
void list<T>::pop_front() {
    auto tmp = ++begin();
    erase(begin(), tmp);
}

template<typename T>
T& list<T>::front() {
    return *begin();
}
template<typename T>
const T& list<T>::front() const {
    return *cbegin();
}
template<typename T>
T& list<T>::back() {
    return *(--end());
}
template<typename T>
const T& list<T>::back() const {
    return *(--cend());
}

template<typename T>
size_t list<T>::size() const {
    size_t n = 0;
    for (auto it = cbegin(); it != cend(); ++it) {
        ++n;
    }
    return n;
}
template<typename T>
bool list<T>::empty() const {
    return cbegin() == cend();
}

template<typename T>
typename list<T>::iterator list<T>::begin() const {
    return iterator(head.r);
}
template<typename T>
typename list<T>::iterator list<T>::end() const {
    return iterator(&head);
}
template<typename T>
typename list<T>::reverse_iterator list<T>::rbegin() const {
    return reverse_iterator(&head);
}
template<typename T>
typename list<T>::reverse_iterator list<T>::rend() const {
    return reverse_iterator(head.r);
}
template<typename T>
typename list<T>::const_iterator list<T>::cbegin() const {
    return const_iterator(head.r);
}
template<typename T>
typename list<T>::const_iterator list<T>::cend() const {
    return const_iterator(&head);
}

template<typename T>
list<T>::~list() {
    clear();
}

template<typename T>
list<T>::iterator::iterator(const iterator &one) : ptr(one.ptr) {}

template<typename T>
typename list<T>::iterator &list<T>::iterator::operator=(const const_iterator &one) {
    list<T>::iterator tmp(one.ptr);
    std::swap(*this, tmp);
    return *this;
}

template<typename T>
bool list<T>::iterator::operator==(const const_iterator &one) const {
    return ptr == one.ptr;
}
template<typename T>
bool list<T>::iterator::operator!=(const const_iterator &one) const {
    return ptr != one.ptr;
}

template<typename T>
T &list<T>::iterator::operator*() {
    return *(ptr->value);
}

template<typename T>
typename list<T>::iterator &list<T>::iterator::operator++() {
    return *this = iterator(this->ptr->r);
}
template<typename T>
typename list<T>::iterator list<T>::iterator::operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
}
template<typename T>
typename list<T>::iterator &list<T>::iterator::operator--() {
    return *this = iterator(this->ptr->l);
}
template<typename T>
typename list<T>::iterator list<T>::iterator::operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
}

template<typename T>
list<T>::iterator::iterator(node *one) : ptr(one) {}

/// const_iterator

template<typename T>
list<T>::const_iterator::const_iterator(const const_iterator &one) : ptr(one.ptr) {}
template<typename T>
list<T>::const_iterator::const_iterator(const iterator &one) : ptr(one.ptr) {}

template<typename T>
bool list<T>::const_iterator::operator==(const const_iterator &one) const {
    return ptr == one.ptr;
}
template<typename T>
bool list<T>::const_iterator::operator!=(const const_iterator &one) const {
    return ptr != one.ptr;
}

template<typename T>
const T &list<T>::const_iterator::operator*() {
    return *(ptr->value);
}

template<typename T>
typename list<T>::const_iterator &list<T>::const_iterator::operator++() {
    return *this = const_iterator(this->ptr->r);
}
template<typename T>
typename list<T>::const_iterator list<T>::const_iterator::operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
}
template<typename T>
typename list<T>::const_iterator &list<T>::const_iterator::operator--() {
    return *this = const_iterator(this->ptr->l);
}
template<typename T>
typename list<T>::const_iterator list<T>::const_iterator::operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
}

template<typename T>
list<T>::const_iterator::const_iterator(node *one) : ptr(one) {}

#endif
