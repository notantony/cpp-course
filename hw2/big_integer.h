#include <vector>
#include <string>
#include <algorithm>
#include <stdint.h>

#define uint128_t unsigned __int128

class big_integer {
private:
	std::vector<uint32_t> data;
	bool sign;
	size_t size() const;
	friend bool check(big_integer const &, big_integer const &, size_t);
	friend void cut(big_integer &, big_integer const &, size_t);
	friend std::vector<uint32_t> shortdiv(big_integer const &, uint32_t);
	friend std::vector<uint32_t> convert(big_integer const &);
	friend big_integer deconvert(std::vector<uint32_t > &);
	friend void invert(std::vector<uint32_t> &);

public:
	//util
	static const big_integer ZERO;

	void swap(big_integer &);

	big_integer();
	explicit big_integer(std::string const &);
	big_integer(big_integer const &);
	big_integer(uint32_t x);
	big_integer(uint64_t x);
	big_integer(int x);

	friend std::string to_string(big_integer const &);

	big_integer operator=(big_integer const &);

	big_integer &operator+=(big_integer const &);
	big_integer &operator-=(big_integer const &);
	big_integer &operator*=(big_integer const &);
	big_integer &operator/=(big_integer const &);
	big_integer &operator&=(big_integer const &);
	big_integer &operator|=(big_integer const &);
	big_integer &operator^=(big_integer const &);
	big_integer &operator%=(big_integer const &);
	big_integer &operator>>=(uint32_t);
	big_integer &operator<<=(uint32_t);

	//unary
	big_integer operator-();
	big_integer operator-() const;
	big_integer operator+();
	big_integer operator~();


	friend bool operator==(big_integer const &, big_integer const &);
	friend bool operator!=(big_integer const &, big_integer const &);
	friend bool operator<(big_integer const &, big_integer const &);
	friend bool operator>(big_integer const &, big_integer const &);
	friend bool operator<=(big_integer const &, big_integer const &);
	friend bool operator>=(big_integer const &, big_integer const &);
};


big_integer operator+(big_integer, big_integer const &);
big_integer operator-(big_integer, big_integer const &);
big_integer operator*(big_integer, big_integer const &);
big_integer operator/(big_integer, big_integer const &);
big_integer operator%(big_integer, big_integer const &);
big_integer operator&(big_integer, big_integer const &);
big_integer operator|(big_integer, big_integer const &);
big_integer operator^(big_integer, big_integer const &);
big_integer operator<<(big_integer, uint32_t);
big_integer operator>>(big_integer, uint32_t);
