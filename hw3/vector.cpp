#include "vector.h"
#include <exception>
#include <algorithm>

vector::vector(): isEmpty(true) {}
vector::vector(size_t size) : vector(size, 0) {}
vector::vector(size_t size, uint32_t one) {
    switch (size) {
    case 0:
        isEmpty = true;
        break;
    case 1:
        isEmpty = false;
        isSmall = true;
        num = one;
        break;
    default:
        isEmpty = false;
        isSmall = false;
        new (&shared) std::shared_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>(size, one));
        break;
    }
}

vector::vector(vector const &one) {
    if (one.isEmpty) {
        isEmpty = one.isEmpty;
        return;
    }
    if (one.isSmall) {
        isEmpty = one.isEmpty;
        isSmall = one.isSmall;
        num = one.num;
        return;
    }
    isEmpty = one.isEmpty;
    isSmall = one.isSmall;
    new (&shared) std::shared_ptr<std::vector<uint32_t>>(one.shared);
}

size_t vector::size() const {
    if (isEmpty) {
        return 0;
    }
    if (isSmall) {
        return 1;
    }
    return shared.get()->size();
}

void vector::operator=(vector const& one) {
    vector tmp(one);
    swap(tmp);
    //swap(*this, tmp);
}

uint32_t vector::back() const {
    if (isEmpty) {
        throw std::runtime_error("Error: .back() called on empty vector");
    }
    if (isSmall) {
         return num;
    }
    return shared.get()->back();
}

uint32_t const& vector::operator[](size_t i) const {
    if (isEmpty) {
        throw std::runtime_error("Error: operator[] called on empty vector");
    }
    if (isSmall) {
        if (i == 0) {
            return num;
        } else {
            throw std::runtime_error("Error: operator[] out of bound on small vector");
        }
    }
    return (*shared.get())[i];
}

bool vector::operator==(vector const& one) const {
    if (isEmpty || one.isEmpty) {
        return isEmpty == one.isEmpty;
    }
    if (isSmall != isSmall) {
        return false;
    }
    if (isSmall && isSmall) {
        return num == one.num;
    }
    return *shared.get() == *one.shared.get();
}

uint32_t& vector::operator[](size_t i) {
    if (isEmpty) {
        throw std::runtime_error("Error: operator[] called on empty vector");
    }
    if (isSmall) {
        if (i == 0) {
            return num;
        } else {
            throw std::runtime_error("Error: operator[] out of bound on small vector");
        }
    }
    if(shared.use_count() != 1) {
        shared.reset(new std::vector<uint32_t>(*shared.get()));
        //shared = make_shared<std::vector<uint32_t>>(*shared.get());
    }
    return (*shared.get())[i];
}

void vector::pop_back() {
    if (isEmpty) {
        throw std::runtime_error("Error: .pop_back() called on empty vector");
    }
    if (isSmall) {
        isEmpty = true;
        return;
    }
    if (shared.use_count() != 1) {
        shared.reset(new std::vector<uint32_t>(*shared.get()));
    }
    if (shared.get()->size() == 2) {
        isSmall = 1;
        uint32_t tmp = (*shared.get())[0];
        shared.reset();
        num = tmp;
        return;
    }
    shared.get()->pop_back();
}

void vector::push_back(uint32_t x) {
    if (isEmpty) {
        isEmpty = false;
        isSmall = true;
        num = x;
        return;
    }
    if (isSmall) {
        isSmall = false;
        //new (&shared)
        //shared = std::make_shared<std::vector<uint32_t>>(1, num);
        new (&shared) std::shared_ptr<std::vector<uint32_t>>(new std::vector<uint32_t>(1, num));
        shared.get()->push_back(x);
        return;
    }
    if (shared.use_count() != 1) {
        shared.reset(new std::vector<uint32_t>(*shared.get()));
    }
    shared.get()->push_back(x);
}

void vector::reverse() {
    if (isEmpty || isSmall) {
       return;
    }
    if (shared.use_count() != 1) {
        shared.reset(new std::vector<uint32_t>(*shared.get()));
    }
    std::reverse(shared.get()->begin(), shared.get()->end());
}

void vector::resize(size_t size) {
    resize(size, 0);
}

void vector::resize(size_t size, uint32_t one) {
    vector tmp = vector(size, one);
    swap(tmp);
    //swap(*this, tmp);
}

vector::~vector() {
    if (isEmpty || isSmall) {
        return;
    }
    shared.reset();
}

void vector::swap(vector& one) {
     std::swap(isEmpty, one.isEmpty);
     std::swap(isSmall, one.isSmall);
     std::swap(shared, one.shared);
}

