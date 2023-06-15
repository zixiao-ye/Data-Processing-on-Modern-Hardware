//
// Created by Alexander Beischl on 2020-05-19.
//

#ifndef HW3_RELATION_HPP
#define HW3_RELATION_HPP
//---------------------------------------------------------------------------
#include <immintrin.h>
#include <stdint.h>

#include <vector>

// Feel free to use something different here, e.g, row vs. column store
constexpr int payload_count = 1;
// Feel free to use something different here, e.g, uint32_t or uint16_t
using keyType = uint64_t;

struct tuple_t {
  keyType key;
  keyType value[payload_count];
};

using relation = std::vector<tuple_t>;

struct partition {
  relation r;
  std::vector<uint64_t> s; // Vector containing the starts of the partitions
};
//---------------------------------------------------------------------------
// Helper struct for partitioning the tuples
struct SplitHelper {
  unsigned bits;
  unsigned fanOut;
  unsigned mask;

  SplitHelper(unsigned bits) : bits(bits) {
    fanOut = 1 << bits;
    mask = fanOut - 1;
  }
};
//---------------------------------------------------------------------------
// Helper struct for the software-managed buffers
unsigned constexpr tuplesPerCL = 64 / sizeof(tuple_t);
union SoftwareManagedBuffer {
  char raw[64];
  tuple_t tuples[tuplesPerCL];
  uint64_t bits[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};
//---------------------------------------------------------------------------
// Helper function for the non-temporal writes
static void
__attribute__((always_inline)) inline storeNontemp(uint8_t *dst,
                                                   SoftwareManagedBuffer *src,
                                                   unsigned cnt = 1)
// Non temporal write of cacheline
{
  while (cnt--) {
#ifdef __AVX512F__
        __m512i *d = (__m512i *)dst;
        __m512i s = *((__m512i *)src);

        __mm512_stream_si512(d, s);
#elif defined(__AVX__)
        __m256i *d1 = (__m256i *)dst;
        __m256i s1 = *((__m256i *)src);
        __m256i *d2 = d1 + 1;
        __m256i s2 = *(((__m256i *)src) + 1);

        __mm256_stream_si256(d1, s1);
        __mm256_stream_si256(d2, s2);
#else
    auto dstCL = reinterpret_cast<SoftwareManagedBuffer *>(dst);
    *dstCL = *(SoftwareManagedBuffer *)src;
#endif
    dst += sizeof(SoftwareManagedBuffer);
    src += 1;
  };
}
//---------------------------------------------------------------------------
#endif  // HW3_RELATION_HPP
