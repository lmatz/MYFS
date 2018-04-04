#include <data/dir.hpp>

namespace myfs {

	Dir::Dir(char* data, size_t data_block_size, const std::vector<int>& indices) : start(data), data_block_size(data_block_size) {


	}

}
