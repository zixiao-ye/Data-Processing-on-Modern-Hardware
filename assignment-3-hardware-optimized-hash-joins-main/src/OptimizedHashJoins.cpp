//
// Created by Alexander Beischl on 2020-05-19.
//
#include "OptimizedHashJoins.hpp"
#include "perfEvent.hpp"
#include <algorithm>
#include <bits/stdc++.h>
#include <functional>
#include <random>
#include <unordered_set>

const std::hash<keyType> hash;

// Assume relation r is smaller, return number of matched tuples
uint64_t hash_join(relation& r, relation& s) {

    // TODO implement the hash join here
    std::unordered_set<keyType> hash_table;
    // Step 1: build phase
    for (const auto& tuple : r) {
        hash_table.emplace(tuple.key);
    }

    // Step 2: probe phase
    uint64_t cnt = 0;
    for (const auto& tuple : s) {
        if (hash_table.find(tuple.key) != hash_table.end())
            cnt++;
    }

    return cnt;
}

partition partition_naive(relation& r, size_t start, size_t end, uint8_t bits,
                          uint8_t shift) {

    // TODO implement the naive partitioning here (shift option will be helpful
    // for multi-pass partitioning)
    SplitHelper split_helper(bits);

    // Step 1 build histograms -> Prefix sum
    std::vector<size_t> histogram(split_helper.fanOut, 0);
    for (size_t i = start; i < end; i++) {
        ++histogram[hash(r[i].key >> shift) & split_helper.mask];
    }

    // Step 2 use prefix sum to partition data
    std::vector<uint64_t> starts(split_helper.fanOut, 0);
    size_t offset = 0;
    for (size_t i = 0; i < split_helper.fanOut; i++) {
        starts[i] = offset;
        // std::cout << offset << std::endl;
        offset += histogram[i];
    }

    // Step 3 partition
    // deep copy of the start points postions
    std::vector<uint64_t> starts_copy = starts;
    relation p;
    p.resize(end - start);
    for (size_t i = start; i < end; i++) {
        auto pos = hash(r[i].key >> shift) & split_helper.mask;
        p[starts_copy[pos]] = r[i];
        starts_copy[pos]++;
    }

    return partition{p, starts};
}

partition partition_naive(relation& r, uint8_t bits) {
    return partition_naive(r, 0, r.size(), bits);
}

std::pair<partition, std::vector<uint64_t>>
partition_softwareManaged(relation& r, uint8_t bits) {

    // TODO implement partitioning with “software-managed buffers” and
    // “non-temporal writes” here Returns the partition with the partition
    // offsets
    SplitHelper split_helper(bits);

    // Step 1 build histograms -> Prefix sum
    std::vector<size_t> histogram(split_helper.fanOut, 0);
    for (size_t i = 0; i < r.size(); i++) {
        ++histogram[hash(r[i].key) & split_helper.mask];
    }

    // Step 2 use prefix sum to partition data
    std::vector<uint64_t> starts(split_helper.fanOut, 0);
    size_t offset = 0;
    for (size_t i = 0; i < split_helper.fanOut; i++) {
        while (offset % tuplesPerCL != 0) {
            offset++;
        }
        starts[i] = offset;
        offset += histogram[i];
    }

    // Step 3 partition
    // deep copy of the start points postions
    std::vector<uint64_t> starts_copy = starts;
    // std::vector<uint64_t> offsets(split_helper.fanOut, 0);

    std::vector<SoftwareManagedBuffer> buffer;
    buffer.resize(split_helper.fanOut);

    // number of tuples in buffer
    std::vector<size_t> num_in_buffer(split_helper.fanOut, 0);

    relation p;
    p.resize(offset);
    for (size_t i = 0; i < r.size(); i++) {
        auto pos = hash(r[i].key) & split_helper.mask;
        buffer[pos].tuples[starts_copy[pos] % tuplesPerCL] = r[i];
        num_in_buffer[pos]++;
        starts_copy[pos]++;

        if (num_in_buffer[pos] == tuplesPerCL) {
            storeNontemp(
                reinterpret_cast<uint8_t*>(&p[starts_copy[pos] - tuplesPerCL]),
                &buffer[pos]);
            num_in_buffer[pos] = 0;
        }
    }

    // flush out remaining tuples in buffer
    for (size_t i = 0; i < split_helper.fanOut; i++) {
        if (num_in_buffer[i] != 0) {
            for (size_t j = 0; j < num_in_buffer[i]; j++) {
                p[starts_copy[i] - num_in_buffer[i] + j] = buffer[i].tuples[j];
            }
        }
    }

    return std::make_pair(partition{p, starts}, starts_copy);
}

partition partition_multiPass(relation& r, uint8_t bits1, uint8_t bits2) {

    // TODO implement Multi-pass partitioning
    // Hint: you may reuse the naive- or the software managed buffers
    // partitioning- and extend it Partition first stage

    // pass 1
    auto p1 = partition_naive(r, 0, r.size(), bits1, 0);

    // pass 2
    std::vector<size_t> starts;
    p1.s.push_back(r.size());
    relation p;
    p.reserve(p1.r.size());
    for (size_t i = 0; i < p1.s.size() - 1; i++) {
        auto p2 = partition_naive(p1.r, p1.s[i], p1.s[i + 1], bits2, bits1);

        // set offset, which is the local start position of the current patition
        // in the whole relation
        auto offset = p1.s[i];
        std::for_each(p2.s.begin(), p2.s.end(),
                      [offset](uint64_t& pos) { pos += offset; });
        p.insert(p.end(), p2.r.begin(), p2.r.end());
        starts.insert(starts.end(), p2.s.begin(), p2.s.end());
    }

    return partition{p, starts};
}

// Assume relation r is smaller, return number of matched tuples
uint64_t radix_join(relation& r, relation& s, Partitioning p) {

    // TODO implement the radix join here

    partition p_r, p_s;
    std::vector<uint64_t> offsets_r, offsets_s;
    uint8_t bits1{14}, bits2{7};
    // Step 1: partitioning phase
    switch (p) {
    case Partitioning::naive:
        p_r = partition_naive(r, bits1);
        p_s = partition_naive(s, bits1);
        break;
    case Partitioning::softwareManaged:
        std::tie(p_r, offsets_r) = partition_softwareManaged(r, bits1);
        std::tie(p_s, offsets_s) = partition_softwareManaged(s, bits1);
        break;
    case Partitioning::multiPass:
        p_r = partition_multiPass(r, bits2, bits2);
        p_s = partition_multiPass(s, bits2, bits2);
        break;
    default:
        break;
    }
    p_r.s.push_back(r.size());
    p_s.s.push_back(s.size());

    // Step 2: partition-wise build & probe phase
    uint64_t cnt{0};

    // index for the last tuple of current partition
    size_t index_r, index_s;
    for (size_t i = 0; i < p_r.s.size() - 1; i++) {
        std::unordered_set<keyType> hash_table;
        // Step 1: build phase
        if (p == Partitioning::softwareManaged) {
            index_r = offsets_r[i];
            index_s = offsets_s[i];
        } else {
            index_r = p_r.s[i + 1];
            index_s = p_s.s[i + 1];
        }

        for (size_t r_pos = p_r.s[i]; r_pos < index_r; r_pos++) {
            hash_table.emplace(p_r.r[r_pos].key);
        }

        // Step 2: probe phase
        for (size_t s_pos = p_s.s[i]; s_pos < index_s; s_pos++) {
            if (hash_table.find(p_s.r[s_pos].key) != hash_table.end())
                cnt++;
        }
    }

    return cnt;
}
