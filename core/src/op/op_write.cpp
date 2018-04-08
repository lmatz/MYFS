#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>



namespace myfs {

	int op_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
		Log::log_msg("op_write path:%s, size:%lu, offset:%lu\n");

		return 0;
	}

}
