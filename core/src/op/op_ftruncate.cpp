#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>




namespace myfs {

	int op_ftruncate(const char *path, off_t offset, struct fuse_file_info *) {
		Log::log_msg("op_ftruncate path:%s, offset:%lu\n");
		int res = 0;

		return 0;
	}

}
