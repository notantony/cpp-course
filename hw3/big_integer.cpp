#include "big_integer.h"
#include <algorithm>
#include <assert.h>
#include <exception>
#include <stdexcept>


typedef unsigned __int128 uint128_t;
//util

size_t big_integer::size() const {
	return data.size();
}

//code

const big_integer big_integer::ZERO = big_integer();

void big_integer::swap(big_integer &one) {
    data.swap(one.data);
    std::swap(sign, one.sign);
}

std::string to_string(big_integer const &one) {
	std::string s;
	big_integer tmp = one;
	tmp.sign = 0;
	while (tmp > 0) {
        vector t = (tmp % 10).data;
		s += '0' + (t.size() == 0 ? 0 : t[0]);
		tmp /= 10;
	}
	if (s.size() == 0)
		s = "0";
	if (one.sign)
		s += '-';
	reverse(s.begin(), s.end());
	return s;
}

big_integer::big_integer() : data(vector()), sign(0) {}
big_integer::big_integer(int x) : data(vector()), sign(0) {
	if (x == 0) {
		return;
	}
	if (x == INT32_MIN) {
		data.push_back((uint32_t) -((int64_t) INT32_MIN));
		sign = 1;
		return;
	}
	if (x < 0) {
		data.push_back((uint32_t) -x);
		sign = 1;
		return;
	}
	data.push_back((uint32_t) x);
}
big_integer::big_integer(uint64_t x) : data(vector()), sign(0) {
	data.push_back((uint32_t) (x & (UINT32_MAX)));
	data.push_back((uint32_t) ((x & (UINT64_MAX)) >> 32));
}

big_integer::big_integer(uint32_t x) : data(vector()), sign(0) {
	if (x != 0)
		data.push_back(x);
}

big_integer::big_integer(std::string const &s) : data(vector()), sign(0) {
	uint64_t sum = 0;
	if (s.size() == 0 || s == "0" || s == "-0") {
		return;
	}
	size_t i = 0;
	if (s[0] == '-') {
		++i;
	}
	for (; i < s.size(); ++i) {
		*this *= 10;
		*this += (s[i] - '0');
	}
	if (s[0] == '-') {
		sign = 1;
	}
	if (sum > 0) {
		data.push_back((uint32_t) sum);
	}
}
big_integer::big_integer(big_integer const &one) : data(vector(one.data)), sign(one.sign) {}

big_integer big_integer::operator=(big_integer const &one) {
	big_integer tmp(one);
	swap(tmp);
	return *this;
}


//logic
bool operator==(big_integer const &a, big_integer const &b) {
	if (a.sign == b.sign && a.data == b.data)
		return 1;
	return 0;
}
bool operator!=(big_integer const &a, big_integer const &b) {
	return !(a == b);
}
bool operator>=(big_integer const &a, big_integer const &b) {
	return (a > b || a == b);
}
bool operator<=(big_integer const &a, big_integer const &b) {
	return (a < b || a == b);
}
bool operator>(big_integer const &a, big_integer const &b) {
	if (a.sign ^ b.sign)
		return !a.sign;
	if (a.size() != b.size()) {
		return (a.size() > b.size()) ^ a.sign;
	}
	size_t i = a.size();
	while (i--) {
		if (a.data[i] != b.data[i]) {
			return (a.data[i] > b.data[i]) ^ a.sign;
		}
	}
	return false;
}
bool operator<(big_integer const &a, big_integer const &b) {
	return !(a > b || a == b);
}

big_integer big_integer::operator-() {
	if (*this == ZERO) {
		return big_integer(*this);
	}
	big_integer tmp(*this);
	tmp.sign = !tmp.sign;
	return tmp;
}
big_integer big_integer::operator-() const {
	if (*this == ZERO) {
		return big_integer(*this);
	}
	big_integer tmp(*this);
	tmp.sign = !tmp.sign;
	return tmp;
}
big_integer big_integer::operator+() {
	return big_integer(*this);
}

//?=
big_integer &big_integer::operator+=(big_integer const &one) {
	if (sign && !one.sign) {
		big_integer tmp = one - (-*this);
		swap(tmp);
		return *this;
	}
	if (!sign && one.sign) {
		big_integer tmp = *this - (-one);
		swap(tmp);
		return *this;
	}
	big_integer tmp;
	bool carry = 0;
	for (size_t i = 0; i < std::max(size(), one.size()); ++i) {
		uint32_t a = (i < size() ? data[i] : 0);
		uint32_t b = (i < one.size() ? one.data[i] : 0);
		uint64_t c = ((uint64_t) a + b + carry);
		if (c > UINT32_MAX) {
			carry = 1;
		} else {
			carry = 0;
		}
		tmp.data.push_back((uint32_t) (c & UINT32_MAX));
	}
	if (carry) {
		tmp.data.push_back(1);
	}
	tmp.sign = sign;
	swap(tmp);
	return *this;
}


big_integer &big_integer::operator-=(big_integer const &one) {
	if (sign && one.sign) {
		big_integer tmp = (-one) - (-*this);
		swap(tmp);
		return *this;
	}
	if (!sign && one.sign) {
		big_integer tmp = *this + (-one);
		swap(tmp);
		return *this;
	}
	if (sign && !one.sign) {
		big_integer tmp = *this + (-one);
		swap(tmp);
		return *this;
	}
	if (*this < one) {
		big_integer tmp = -(one - *this);
		swap(tmp);
		return *this;
	}
	big_integer tmp = big_integer();
	bool borrow = 0;
	for (size_t i = 0; i < size(); ++i) {
		uint32_t a = (i < size() ? data[i] : 0);
		uint32_t b = (i < one.size() ? one.data[i] : 0);
		int64_t c = (int64_t) a - b - borrow;
		if (c < 0) {
			c += UINT32_MAX + 1;
			borrow = 1;
		} else {
			borrow = 0;
		}
		tmp.data.push_back((uint32_t) c);
	}
	while (tmp.size() > 0 && tmp.data.back() == 0) {
		tmp.data.pop_back();
	}
	tmp.sign = 0;
	swap(tmp);
	return *this;
}
big_integer &big_integer::operator*=(big_integer const &one) {
	if (*this == ZERO || one == ZERO) {
		big_integer tmp = ZERO;
		swap(tmp);
		return *this;
	}
	big_integer tmp = big_integer();
	tmp.data.resize(size() + one.size(), 0);
	for (size_t i = 0; i < size(); ++i) {
		uint32_t carry = 0;
		for (size_t j = 0; j < one.size(); ++j) {
			uint32_t a = data[i];
			uint32_t b = one.data[j];
			uint64_t c = (uint64_t) a * b + tmp.data[i + j] + carry;
			tmp.data[i + j] = (uint32_t) (c & (UINT32_MAX));
			carry = (uint32_t) (c >> 32);
		}
		if (carry) {
			tmp.data[i + one.size()] += carry;
		}
	}
	while (tmp.size() > 0 && tmp.data.back() == 0)
		tmp.data.pop_back();
	tmp.sign = this->sign ^ one.sign;
	swap(tmp);
	return *this;
}


bool check(big_integer const &a, big_integer const &b, size_t index) {
	for (size_t i = 0; i < a.size(); i++) {
		if (a.data[a.size() - i - 1] != (index - i - 1 < b.size() ? b.data[index - i - 1] : 0))
			return a.data[a.size() - i - 1] > (index - i - 1 < b.size() ? b.data[index - i - 1] : 0);
	}
	return true;
}
void cut(big_integer &a, big_integer const &b, size_t index) {
	size_t start = a.size() - index;
	bool borrow = 0;
	for (size_t i = 0; i < index; ++i) {
		uint32_t x = a.data[start + i];
		uint32_t y = (i < b.size() ? b.data[i] : 0);
		int64_t c = (int64_t) x - y - borrow;
		if (c < 0) {
			c += UINT32_MAX + 1;
			borrow = 1;
		} else {
			borrow = 0;
		}
		a.data[start + i] = (uint32_t) c;
	}
}

vector shortdiv(big_integer const &a, uint32_t b) {
    vector tmp;
	uint64_t rest = 0;
	uint64_t x;
	for (size_t i = 0; i < a.size(); i++) {
		x = (rest << 32) | a.data[a.size() - 1 - i];
		tmp.push_back((uint32_t) (x / b));
		rest = x % b;
    }
    tmp.reverse();
    while (tmp.size() && !tmp.back())
		tmp.pop_back();
	return tmp;
}


big_integer &big_integer::operator/=(big_integer const &one) {
	big_integer a = *this;
	big_integer b = one;
	a.sign = 0;
	b.sign = 0;

	if (b == ZERO) {
		throw std::runtime_error("divide by zero");
	}
	if (a < b) {
		big_integer tmp = ZERO;
		swap(tmp);
		return *this;
	}

	uint64_t base = ((uint64_t) 1) << 32;

	big_integer tmp;
	if (b.size() == 1) {
		tmp.data = shortdiv(a, b.data[0]);
		if (tmp.size())tmp.sign = sign ^ one.sign;
		swap(tmp);
		return *this;
	}

	uint64_t d = base / one.data.back();
	a *= d;
	b *= d;

	a.data.push_back(0);
	a.data.push_back(0);

	size_t m = b.size() + 1;
	size_t n = a.size();
	tmp.data.resize(n - m + 1);
	big_integer qd;
	uint32_t res;
	for (size_t i = m, j = tmp.data.size() - 1; i <= n; ++i, --j) {


		uint128_t x = (((uint128_t) a.data[a.size() - 1] << 64) |
		               ((uint128_t) a.data[a.size() - 2] << 32) |
		               ((uint128_t) a.data[a.size() - 3]));
		uint128_t y = (((uint128_t) b.data[b.size() - 1] << 32) |
		               (uint128_t) b.data[b.size() - 2]);

		res = std::min((uint32_t) (x / y), UINT32_MAX);

		qd = b * res;

		if (!check(a, qd, m)) {
			res--;
			qd -= b;
		}
		tmp.data[j] = res;
		cut(a, qd, m);
		if (!a.data.back())
			a.data.pop_back();
	}

	while (tmp.size() && !tmp.data.back())tmp.data.pop_back();
	if (tmp.size())tmp.sign = sign ^ one.sign;
	swap(tmp);
	return *this;
}
big_integer &big_integer::operator%=(big_integer const &one) {
	big_integer tmp = *this / one;
	tmp *= one;
	tmp = *this - tmp;
	swap(tmp);
	return *this;
}

void invert(vector &one) {
	for (size_t i = 0; i < one.size(); ++i) {
		one[i] = ~one[i];
	}
}


vector convert(big_integer const &one) {
	if (!one.sign)
		return one.data;
    vector tmp = one.data;
	invert(tmp);
	for (size_t i = 0; i < tmp.size(); i++) {
		if (tmp[i] != UINT32_MAX) {
			tmp[i]++;
			return tmp;
		}
		tmp[i] = 0;
	}
	tmp.push_back(1);
	return tmp;
}

big_integer deconvert(vector &one) {
	big_integer tmp;
	if (!(one.back() & (1 << 31))) {
		tmp.data = one;
		return tmp;
	}
	invert(one);
	tmp.sign = 1;
	for (size_t i = 0; i < one.size(); i++) {
		if (one[i] != UINT32_MAX) {
			one[i]++;
			tmp.data = one;
			return tmp;
		}
		one[i] = 0;
	}
	one.push_back(1);
	tmp.data = one;
	return tmp;
}

big_integer big_integer::operator~() {
    vector tmp = convert(*this);
	invert(tmp);
	big_integer ret = deconvert(tmp);
	swap(ret);
	return *this;
}

big_integer &big_integer::operator&=(big_integer const &one) {
	big_integer tmp;
    vector a = convert(*this);
    vector b = convert(one), c(std::max(a.size(), b.size()));
	for (size_t i = 0; i < std::max(a.size(), b.size()); i++) {
		c[i] = (i < a.size() ? a[i] : 0) & (i < b.size() ? b[i] : 0);
	}
	tmp = deconvert(c);
	while (tmp.data.size() && !tmp.data.back())
		tmp.data.pop_back();
	swap(tmp);
	return *this;
}
big_integer &big_integer::operator^=(big_integer const &one) {
	big_integer tmp;
    vector a = convert(*this);
    vector b = convert(one), c(std::max(a.size(), b.size()));
	for (size_t i = 0; i < std::max(a.size(), b.size()); i++) {
		c[i] = (i < a.size() ? a[i] : 0) ^ (i < b.size() ? b[i] : 0);
	}
	tmp = deconvert(c);
	while (tmp.data.size() && !tmp.data.back())
		tmp.data.pop_back();
	swap(tmp);
	return *this;
}
big_integer &big_integer::operator|=(big_integer const &one) {
	big_integer tmp;
    vector a = convert(*this);
    vector b = convert(one), c(std::max(a.size(), b.size()));
	for (size_t i = 0; i < std::max(a.size(), b.size()); i++) {
		c[i] = (i < a.size() ? a[i] : 0) | (i < b.size() ? b[i] : 0);
	}
	tmp = deconvert(c);
	while (tmp.data.size() && !tmp.data.back())
		tmp.data.pop_back();
	swap(tmp);
	return *this;
}
big_integer &big_integer::operator>>=(uint32_t x) {
	big_integer tmp = *this;
    tmp.data.reverse();
	for (uint32_t i = 0; tmp.size() && i < x / 32; i++) {
		tmp.data.pop_back();
	}
    tmp.data.reverse();
	tmp.sign=0;
	if(sign) {
		tmp += ((uint32_t) 1 << (x % 32)) - 1;
	}
	tmp /= ((uint32_t) 1 << (x % 32));
	tmp.sign = sign;
	swap(tmp);
	return *this;
}
big_integer &big_integer::operator<<=(uint32_t x) {
	big_integer tmp = *this * ((uint32_t)1 << (x % 32));
    tmp.data.reverse();
	for (uint32_t i = 0; i < x / 32; i++) {
		tmp.data.push_back(0);
	}
    tmp.data.reverse();
	swap(tmp);
	return *this;
}

big_integer operator+(big_integer a, big_integer const &b) {
	return a += b;
}
big_integer operator-(big_integer a, big_integer const &b) {
	return a -= b;
}
big_integer operator*(big_integer a, big_integer const &b) {
	return a *= b;
}
big_integer operator/(big_integer a, big_integer const &b) {
	return a /= b;
}
big_integer operator%(big_integer a, big_integer const &b) {
	return a %= b;
}
big_integer operator&(big_integer a, big_integer const &b) {
	return a &= b;
}
big_integer operator|(big_integer a, big_integer const &b) {
	return a |= b;
}
big_integer operator^(big_integer a, big_integer const &b) {
	return a ^= b;
}
big_integer operator>>(big_integer a, uint32_t b) {
	return a >>= b;
}
big_integer operator<<(big_integer a, uint32_t b) {
	return a <<= b;
}
