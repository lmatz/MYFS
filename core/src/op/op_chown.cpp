#include <utils/log.hpp>
#include <myfs/myfs.hpp>

namespace myfs {

	int op_chown(const char *path, uid_t uid, gid_t gid) {
		Log::log_msg("op_chown path:%s uid_t:%d gid_t:%d", path, uid, gid);
		return 0;
	}

}
