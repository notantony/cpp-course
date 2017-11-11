#include <iostream>
#include <stdio.h>
#include "big_integer.h"
#include <string>

using namespace std;

void f(int x) {
	std::string s;
	for(int i=31;i>=0;i--)
		s+=((bool)(x&(1<<i)))+'0';
	printf("%s %d %u\n",s.c_str(),x,x);
}


int main() {

	f(-1234);
	f((-1234)>>3);
	f(-1234/(8));

	printf("%s\n", to_string(

			big_integer(-1234)>>3

	).c_str());
	return 0;
}
