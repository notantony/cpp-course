#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "bitvector.h"
#include "compressor.h"

const size_t BUFFER_SIZE = 4096;

void test(std::ifstream &input) {
    input.get();
	//std::cout << input.eof() << " " << (int)c;
	if(!input.eof())input.unget();
}

void input_check(std::ifstream &input) {
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	} else if (input.fail()) {
		throw std::runtime_error("Input file is corrupted");
	}
}

void my_writev(std::ofstream &output, const std::vector<unsigned char> &tmp) {
	unsigned char buffer[BUFFER_SIZE];
    for (size_t i = 0; i < tmp.size(); /*i++*/) {
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
	unsigned char buffer[BUFFER_SIZE];
	char c;
	int first;
	input.get(c);
	input_check(input);
	first = reinterpret_cast<unsigned char&>(c);
	size_t len = 0;
    for (size_t i = 0; i < sizeof(size_t); i++) {//TODO
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
	unsigned char buffer[BUFFER_SIZE];
	while (input.good()) {
		input.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
		for (int i = 0; i < input.gcount(); i++) {
			cnt[(buffer[i])]++;
		}
	}
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	}
	compressor compressor(cnt);
	
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
	unsigned char buffer[BUFFER_SIZE];
	input.read(reinterpret_cast<char*>(buffer), 256);
	input_check(input);
	std::vector<unsigned char> tmp(256);
	for (int i = 0; i < 256; i++) {
		tmp[i] = buffer[i];
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

#include<ctime>

int main(int n, char* args[]) {

    int time = clock();

	if (n != 4) {
		throw std::runtime_error("Wrong number of arguments\nFormat: \"<c/d> <input filename> <output filename>\" \nUse \"c\" for compressing, \"d\" for decompressing");
		return 1;
	}
	std::ifstream input(args[2], std::ios_base::binary);
	if (!input.is_open()) {
		throw std::runtime_error("Cannot open file for reading\n");
	}
	std::ofstream output(args[3], std::ios_base::binary);
	if (!output.is_open()) {
		throw std::runtime_error("Cannot open file for writing\n");
	}
	if (!strcmp(args[1], "c")) {
		compress(input, output);
	} else if (!strcmp(args[1], "d")) {
		decompress(input, output);
	} else {
		throw std::runtime_error("Wrong first parameter, use \"c\" for compresssion, \"d\" for  decompression");
	}
	input.close();
	output.close();


    std::cout << clock() - time;
	return 0;
}

