#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "bitvector.h"
#include "compressor.h"
#include <stdexcept>
#include <ctime>

const size_t BUFFER_SIZE = 4096 * 8;
static unsigned char buffer[BUFFER_SIZE];

void test(std::ifstream &input) {
    input.get();
	if(!input.eof())input.unget();
}

void input_check(std::ifstream &input) {
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	} else if (input.fail()) {
		throw std::runtime_error("Error: input file is corrupted");
	}
}

void my_writev(std::ofstream &output, const std::vector<unsigned char> &tmp) {
    for (size_t i = 0; i < tmp.size();) {
        size_t j = 0;
		for (; j < BUFFER_SIZE && i < tmp.size(); j++, i++) {
			buffer[j] = tmp[i];
		}
		if (output.good()) {
			output.write(reinterpret_cast<char*>(buffer), j);
		}
		if (output.fail()) {
			throw std::runtime_error("Error while writing compressed file");
		}
	}
}

bitreader my_readv(std::ifstream &input) {
	char c;
	int first;
	input.get(c);
	input_check(input);
	first = reinterpret_cast<unsigned char&>(c);
	size_t len = 0;
    for (size_t i = 0; i < sizeof(size_t); i++) {
		input.get(c);
		input_check(input);
		len = (len << 8) | reinterpret_cast<unsigned char&>(c);
	}
	len -= sizeof(size_t) + 1;
	std::vector<unsigned char> tmp;
	while (len > 0) {
		input.read(reinterpret_cast<char*>(buffer), std::min(sizeof(buffer), len));
		input_check(input);
		len -= input.gcount();
		for (int i = 0; i < input.gcount(); i++) {
			tmp.push_back(buffer[i]);
		}
	}
	return bitreader(first, tmp);
}


void compress(std::ifstream &input, std::ofstream &output) {
	std::vector<uint64_t> cnt(256 + 255, 0);
	cnt.shrink_to_fit();
	while (input.good()) {
		input.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
		for (int i = 0; i < input.gcount(); i++) {
			cnt[buffer[i]]++;
		}
	}
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	}
	compressor compressor(cnt);

	double progress_ttl = 0, progress_size = 0;
	for (size_t i = 0; i < 256; i++) {
		progress_ttl += cnt[i];
		progress_size += compressor.get_code(i).size() * cnt[i];
	}
    std::cout << "Approximate ouput file size is " << progress_size / 8 / 1024 << " kB\n";

	my_writev(output, compressor.get_leaves_code());
	bitvector bv;
	bv.push(compressor.get_tree_code());
	my_writev(output, bv.release());
	bv.clear();

	input.clear();
	input.seekg(0, input.beg);
	std::vector<unsigned char> tmp;
	test(input);
	while (input.good()) {
		input.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
		for (int i = 0; i < input.gcount(); i++) {
			bv.push(compressor.get_code(buffer[i]));
		}
		my_writev(output, bv.release());
		bv.clear();
		test(input);
	}
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	}
}

void decompress(std::ifstream &input, std::ofstream &output) {
	input.read(reinterpret_cast<char*>(buffer), 256);
	input_check(input);
	char check[256];
	memset(check, 0, 256);
	std::vector<unsigned char> tmp(256);
	for (int i = 0; i < 256; i++) {
		tmp[i] = buffer[i];
		if (check[buffer[i]] != 0) {
			throw std::runtime_error("Error: input file is corrupted");
		}
		check[buffer[i]] = 1;
	}
	compressor compressor(my_readv(input), tmp);
	test(input);
	while (input.good()) {
		bitreader br = my_readv(input);
		tmp.clear();
		while (!br.ended()) {
			tmp.push_back(compressor.get_char(br));
		}
		my_writev(output, tmp);
		test(input);
	}
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	}
}



int main(int n, char* args[]) {
	try {
		double start_time = clock();
        if (n == 2 && strcmp(args[1], "help") == 0) {
            std::cout << "Format: \"<c/d> <input filename> <output filename>\" \nUse \"c\" for compressing, \"d\" for decompressing";
        }
		if (n != 4) {
			std::cout << "Wrong number of arguments\nFormat: \"<c/d> <input filename> <output filename>\" \nUse \"c\" for compressing, \"d\" for decompressing";
			return 1;
		}
		int mode = -1;

		if (strcmp(args[1], "c") == 0) {
			mode = 1;
		} else if (strcmp(args[1], "d") == 0) {
			mode = 2;
		} else {
			std::cout << "Wrong first parameter, use \"c\" for compresssion, \"d\" for  decompression";
			return 1;
		}

		std::ifstream input(args[2], std::ios_base::binary);
		if (!input.is_open()) {
			std::cout << "Cannot open file for reading\n";
			return 1;
		}
		std::ofstream output(args[3], std::ios_base::binary);
		if (!output.is_open()) {
			std::cout << "Cannot open file for writing\n";
			return 1;
		}	

		if (mode == 1) {
			compress(input, output);
		} else if (mode == 2) {
			decompress(input, output);
		}

		std::cout << "Done successfully, elapsed time: " << (static_cast<double>(clock()) - start_time) / static_cast<double>(CLOCKS_PER_SEC) << " seconds\n" ;
	} catch (std::runtime_error e){
		std::cout << e.what();
	}
	return 0;
}

