#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>




namespace myfs {

	int op_access(const char *path, int mask) {
		uid_t uid = fuse_get_context()->uid;
		gid_t gid = fuse_get_context()->gid;
		char buf[6];
		straccess(mask, buf);
		Log::log_msg("op_access path:%s mask:%d->%x, with uid:%lu gid:%lu\n", path, mask, buf, uid, gid);
		GlobalMetadata *metadata = MYFS_METADATA;
		int res = 0;		

		res = metadata->check_access(path, mask, uid, gid);
		if (res != 0) {
			Log::log_msg("op_access check_access fails\n");
			return -1;
		}
	
		return 0;
	}

}
