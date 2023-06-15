#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>

#include "OptimizedHashJoins.hpp"
#include "Relation.hpp"

inline std::string getDir(const std::string &file)
/// Returns path to file in 'file'
{
  size_t found = file.find_last_of("/\\");
  return (file.substr(0, found));
}

std::unique_ptr<relation> getRelation(std::string dataDir) {
  std::ifstream relFile;
  relFile.open(dataDir);

  auto rel = std::make_unique<relation>();
  while(!relFile.eof()) {
    tuple_t t;
    relFile >> t.key;
    rel->push_back(t);
  }
  return rel;
}

std::unique_ptr<relation> getR() {
  return getRelation(getDir(__FILE__) + "/data/r.dat");
}
std::unique_ptr<relation> getS() {
  return getRelation(getDir(__FILE__) + "/data/s.dat");
};

TEST(OptimizedHashJoins, HashJoin) {
  auto r = getR();
  auto s = getS();

  auto matches = hash_join(*r, *s);
  ASSERT_EQ(matches, 4096);
}

TEST(OptimizedHashJoins, Partition_Naive) {
  auto s = getS();
  uint8_t bits = 4;
  auto partition = partition_naive(*s, bits);

  ASSERT_EQ(partition.s.size(), 1 << bits);
  ASSERT_EQ(s->size(), partition.r.size());

  std::vector<uint64_t> start_pos = {0, 247, 511, 780, 1041, 1309, 1555, 1805, 2056, 2312, 2542, 2803, 3057, 3291, 3571, 3838};
  ASSERT_EQ(partition.s, start_pos);

  SplitHelper split(bits);
  start_pos.push_back(s->size());
  for (uint64_t index = 0; index < split.fanOut; index++) {
    for (uint64_t partition_index = start_pos[index]; partition_index < start_pos[index+1]; partition_index++) {
      ASSERT_EQ(partition.r[partition_index].key & split.mask, index);
    }
  }
}

TEST(OptimizedHashJoins, RadixJoin_Naive) {
  auto r = getR();
  auto s = getS();

  auto matches = radix_join(*r, *s, Partitioning::naive);
  ASSERT_EQ(matches, 4096);
}

TEST(OptimizedHashJoins, Partition_SoftwareManaged) {
  auto s = getS();
  SplitHelper split(4);
  partition partition{};
  std::vector<uint64_t> offsets;

  std::tie(partition, offsets) = partition_softwareManaged(*s, split.bits);
  ASSERT_EQ(partition.s.size(), split.fanOut);
  ASSERT_TRUE(s->size() <= partition.r.size());

  std::vector<uint64_t> start_pos = {0, 248, 512, 784, 1048, 1316, 1564, 1816, 2068, 2324, 2556, 2820, 3076, 3312, 3592, 3860};
  ASSERT_EQ(partition.s, start_pos);

  start_pos.push_back(s->size());
  for (uint64_t index = 0; index < split.fanOut; index++) {
    auto limit = std::min(offsets[index], start_pos[index+1]);
    for (uint64_t partition_index = start_pos[index]; partition_index < limit; partition_index++) {
      ASSERT_EQ(partition.r[partition_index].key & split.mask, index);
    }
  }
}

TEST(OptimizedHashJoins, RadixJoin_SoftwareManaged) {
  auto r = getR();
  auto s = getS();

  auto matches = radix_join(*r, *s, Partitioning::softwareManaged);
  ASSERT_EQ(matches, 4096);
}

TEST(OptimizedHashJoins, Partition_Naive_WithShift) {
  auto s = getS();
  SplitHelper split(4);
  u_int8_t shift = 4;
  auto partition = partition_naive(*s, 0, s->size(), split.bits, shift);

  ASSERT_EQ(partition.s.size(), split.fanOut);
  ASSERT_EQ(s->size(), partition.r.size());

  std::vector<uint64_t> start_pos = {0, 250, 517, 779, 1052, 1314, 1551, 1827, 2084, 2333, 2605, 2862, 3131, 3364, 3604, 3859};
  ASSERT_EQ(partition.s, start_pos);

  start_pos.push_back(s->size());
  for (uint64_t index = 0; index < split.fanOut; index++) {
    for (uint64_t partition_index = start_pos[index]; partition_index < start_pos[index+1]; partition_index++) {
      ASSERT_EQ((partition.r[partition_index].key >> shift) & split.mask, index);
    }
  }
}

TEST(OptimizedHashJoins, Partition_MultiPass) {
  auto s = getS();
  uint8_t bits = 2;
  auto partition = partition_multiPass(*s, bits, bits);

  SplitHelper split(bits*2);
  ASSERT_EQ(partition.s.size(), split.fanOut);
  ASSERT_EQ(s->size(), partition.r.size());

  std::vector<uint64_t> start_pos = {0, 247, 515, 771, 1005, 1269, 1515, 1745, 2025, 2294, 2544, 2805, 3072, 3333, 3584, 3838};
  ASSERT_EQ(partition.s, start_pos);

  start_pos.push_back(s->size());
  for (uint64_t firstPass_index = 0; firstPass_index < 4; firstPass_index++) {
    for (uint64_t secondPass_index = 0; secondPass_index < 4; secondPass_index++) {
      auto index = firstPass_index * 4 + secondPass_index;
      for (uint64_t partition_index = start_pos[index]; partition_index < start_pos[index+1]; partition_index++) {
        ASSERT_EQ(partition.r[partition_index].key & split.mask, secondPass_index * 4 + firstPass_index);
      }
    }
  }
}

TEST(OptimizedHashJoins, RadixJoin_MultiPass) {
  auto r = getR();
  auto s = getS();

  auto matches = radix_join(*r, *s, Partitioning::multiPass);
  ASSERT_EQ(matches, 4096);
}