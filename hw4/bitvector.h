#include <vector>

class bitvector {
private:
	bool released = false;
	int space;
	std::vector<unsigned char> v;
public:
	void push(const std::vector<unsigned char> &one);
	const std::vector<unsigned char> &release();
	void clear();
	bitvector::bitvector();
};

class bitreader {
private:
	int pos, bit, last_space;
	std::vector<unsigned char> v;
public:
	bitreader(int space, const std::vector<unsigned char> one);
	unsigned char next();
	bool ended() const;
	std::vector<unsigned char> to_vector();
};