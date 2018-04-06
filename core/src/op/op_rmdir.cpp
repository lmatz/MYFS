#include <utils/log.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_rmdir(const char *path) {
		Log::log_msg("op_rmdir path:%s", path);
		return 0;
	}

}
