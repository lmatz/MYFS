#include <utils/log.hpp>
#include <myfs/myfs.hpp>

namespace myfs {

	int op_utime(const char *path, struct utimbuf *buf) {
		Log::log_msg("op_utime should be deprecated. But it gets called!");
		return 0;
	}

}
