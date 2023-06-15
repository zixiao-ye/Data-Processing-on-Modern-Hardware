//
// Created by Alice Rey on 12.04.21.
//

#ifndef HW3_OPTIMIZEDHASHJOINS_HPP
#define HW3_OPTIMIZEDHASHJOINS_HPP

#include "Relation.hpp"

enum class Partitioning { naive, softwareManaged, multiPass };

uint64_t hash_join(relation &r, relation &s);

partition partition_naive(relation &r, size_t start, size_t end, uint8_t bits, uint8_t shift = 0);

partition partition_naive(relation &r, uint8_t bits);

std::pair<partition, std::vector<uint64_t>> partition_softwareManaged(relation &r, uint8_t bits);

partition partition_multiPass(relation &r, uint8_t bits1, uint8_t bits2);

uint64_t radix_join(relation &r, relation &s, Partitioning p);

#endif // HW3_OPTIMIZEDHASHJOINS_HPP
