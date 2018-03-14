#include <utils/log.hpp>
#include <myfs/myfs.hpp>

namespace myfs {
	
	int op_getdir(const char *path, fuse_dirh_t, fuse_dirfil_t) {
		Log::log_msg("getdir should be deprecated, but it gets called!");
		return -1;
	}

}
