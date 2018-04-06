#include <utils/log.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_truncate(const char *path, off_t size) {
		Log::log_msg("op_truncate path:%s off_t:%d", path, size);
		return 0;
	}

}
