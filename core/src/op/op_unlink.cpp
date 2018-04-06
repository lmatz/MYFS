#include <utils/log.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_unlink(const char *path) {
		Log::log_msg("op_unlink path:%s", path);
		return 0;
	}

}
