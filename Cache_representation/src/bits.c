#include "bits.h"
#include "cache.h"

int get_set(Cache *cache, address_type address) {
  // TODO:
  //  Extract the set bits from a 32-bit address.
  //
  unsigned int tag_set = address >> (cache -> block_bits); 
  return (tag_set%cache->set_count);
}

int get_line(Cache *cache, address_type address) {
  // TODO:
  // Extract the tag bits from a 32-bit address.
  //
  unsigned int tag = address >> (cache->block_bits + cache->set_bits);
  return tag;
}

int get_byte(Cache *cache, address_type address) {
  // TODO
  // Extract the block offset (byte index) bits from a 32-bit address.
  //
  return (address % cache->block_size);
}
