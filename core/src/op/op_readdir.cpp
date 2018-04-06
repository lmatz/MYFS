#include <utils/log.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi) {
		Log::log_msg("op_readdir path:%s\n", path);

		return 0;
	}

}
