#include <stdint.h>
#include <stddef.h>
#include <vector>
#include <memory>

struct vector{
    vector();
    vector(size_t);
    vector(size_t, uint32_t);
    vector(vector const&);
    void operator=(vector const&);

    size_t size() const;
    uint32_t back() const;
    uint32_t const &operator[](size_t) const;
    bool operator==(vector const&) const;
    uint32_t &operator[](size_t);
    void pop_back();
    void push_back(uint32_t);
    void reverse();
    void resize(size_t);
    void resize(size_t, uint32_t);
    ~vector();
    void swap(vector &);

private:
    bool isSmall = true, isEmpty = false;
    union {
        std::shared_ptr<std::vector<uint32_t>> shared;
        uint32_t num;
    };

};

