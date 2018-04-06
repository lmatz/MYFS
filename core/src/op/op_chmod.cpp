#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/functions.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_chmod(const char *path, mode_t mode) {
		char buf[10];
		strmode(mode, buf);
		Log::log_msg("op_chmod path:%s mode_t:%s\n", path, buf);
		int res = 0;
		res = MYFS_METADATA->change_mode(path, mode);
		if (res != 0) {
			Log::log_msg("op_chmod path:%s mode_t:%s fails.\n", path, buf);
			return res;
		}
		return 0;
	}

}
