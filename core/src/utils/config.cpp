#include <utils/config.hpp>

namespace myfs {

uint32_t Config::max_write = 4096;

uint64_t Config::theta = 1024*1024*1024; // 1GB

//const uint64_t Config::block_size = 4*1024*1024; // 4KB

}
