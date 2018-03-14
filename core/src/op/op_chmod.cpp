#include <utils/log.hpp>
#include <myfs/myfs.hpp>

namespace myfs {

	int op_chmod(const char *path, mode_t mode) {
		char buf[10];
		strmode(mode, buf);
		Log::log_msg("op_chmod path:%s mode_t:%s", path, buf);
		return 0;
	}

}
