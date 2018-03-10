#ifndef MYLIST_H
#define MYLIST_H

#include <stddef.h>
#include <memory>

template<typename T>
class list {
private:
	struct node {
		std::shared_ptr<node> l, r;
		T value;
		node() {
			l = std::make_shared(this);
			r = l;
		}
	} head;
	size_t size;
public:
	class iterator {
		node *a;
	private:
		T &operator*();
		iterator &operator++();
		iterator &operator--();
	};

	list();
	list(const list<T> &one);

	~list();
	void clear();

};

template<T>
list<T>::list() {};

template<T>
void list<T>::clear() {
	
};


template<T>
T list<T>::iterator::operator*() {
	return value;
};

template<T>
list<T>::iterator &list<T>::iterator::operator++() {
	return this->l.get();
};

template<T>
list<T>::iterator &list<T>::iterator::operator--() {
	return value;
};


#endif