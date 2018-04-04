#include <stdint.h>

#include <utils/type.hpp>

#include <vector>

namespace myfs {

	class Dir {
	public:
		// construct a Dir class by binary data in data blocks, data block size
		Dir(char* data, size_t data_block_size, const std::vector<int>& indices);

	private:
		 const char* start;
		 const size_t data_block_size;
		 std::vector<int> indices;
	};

}
