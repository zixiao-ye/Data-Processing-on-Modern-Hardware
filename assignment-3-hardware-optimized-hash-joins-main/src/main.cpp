#include <algorithm>
#include <bits/stdc++.h>

#include "OptimizedHashJoins.hpp"
#include "Relation.hpp"
#include "perfEvent.hpp"

int main(int argc, char* argv[]) {
    std::srand(std::time(nullptr));

    // TODO: Change the size of the datasets for your benchmark
    size_t r_tuples = 1 << 20;
    size_t s_tuples = r_tuples;

    std::cout << r_tuples << std::endl;

    size_t index = 2; // 0: hj, 1: naive, 2: sw, 3: multi-pass

    std::vector<uint64_t> v(r_tuples);        // vector with r_tuples ints.
    std::iota(std::begin(v), std::end(v), 0); // Fill with 0, 1, ..., r_tuples.
    std::random_shuffle(std::begin(v), std::end(v));

    relation r;
    relation s;

    for (auto i : v) {
        tuple_t t;
        t.key = i;
        r.push_back(t);
    }

    for (size_t i = 0; i < s_tuples; i++) {
        tuple_t t;
        t.key = rand() % r_tuples;
        s.push_back(t);
    }

    BenchmarkParameters params;
    params.setParam("r_tuples", r_tuples);
    params.setParam("s_tuples", s_tuples);
    params.setParam("version", index);

    PerfEventBlock e(r_tuples, params, true);
    switch (index) {
    case 0:
        hash_join(r, s);
        break;
    case 1:
        radix_join(r, s, Partitioning::naive);
        break;
    case 2:
        radix_join(r, s, Partitioning::softwareManaged);
        break;
    case 3:
        radix_join(r, s, Partitioning::multiPass);
        break;
    }
}