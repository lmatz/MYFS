#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>




namespace myfs {

	int op_release(const char *path, struct fuse_file_info *fi) {
		Log::log_msg("op_release path:%s\n");
		int res = 0;

		return 0;
	}

}
