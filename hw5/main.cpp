#include <set>
#include <vector>
#include <algorithm>
#include <assert.h>
#include "persistent_set.h"
#include "linked_ptr.h"
#include "shared_ptr.h"
#include "tests.cpp"

int main() {
    fprintf(stderr, "Corectness tests\n");
    prepare();
    fprintf(stderr, "Testing std::shared_ptr\n");
    run<std::shared_ptr>();
    fprintf(stderr, "Testing shared_ptr\n");
    run<shared_ptr>();
    fprintf(stderr, "Testing linked_ptr\n");
    run<linked_ptr>();


    fprintf(stderr, "Performance tests\n");
    fprintf(stderr, "Testing std::shared_ptr\n");
    run_perf<std::shared_ptr>();
    fprintf(stderr, "Testing shared_ptr\n");
    run_perf<shared_ptr>();
    fprintf(stderr, "Testing linked_ptr\n");
    run_perf<linked_ptr>();

    return 0;
}
