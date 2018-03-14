#pragma once
#include <stdint.h>

namespace myfs {

	class Config {
	public:
		static uint32_t max_write;

		static uint64_t theta;

		static const uint64_t block_size = 4*1024*1024;

		static const uint32_t num_blocks = 15;

	};


}
