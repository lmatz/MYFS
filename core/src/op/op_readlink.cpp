#include <utils/log.hpp>
#include <myfs/myfs.hpp>

namespace myfs {
	
	int op_readlink(const char *path, char *buf, size_t size) {
		Log::log_msg("op_readlink path:%s", path);

		return 0;
	}

}
