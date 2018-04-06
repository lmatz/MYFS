#include <op/op_myfs.hpp>
#include <utils/log.hpp>
#include <utils/macros.hpp>

namespace myfs {

	int op_chown(const char *path, uid_t uid, gid_t gid) {
		int res = 0;
		Log::log_msg("op_chown path:%s uid_t:%d gid_t:%d\n", path, uid, gid);
		
		res = MYFS_METADATA->change_owner(path, uid, gid);
		if (res != 0) {
			Log::log_msg("op_chmod path:%s uid_t:%d gid_t:%d fails.\n", path, uid, gid);
			return res;
		}
		return 0;
	}

}
