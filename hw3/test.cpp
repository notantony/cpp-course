#include <stdio.h>
#include "big_integer.h"


int main() {

    printf("%s\n",to_string(
               big_integer(65536)*
               big_integer(65536)
               ).c_str());

    return 0;
}
