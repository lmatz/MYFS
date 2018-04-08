#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>


namespace myfs {

	int op_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
		char buf[10];
		strmode(mode, buf);
		Log::log_msg("op_create path:%s, mode:%s\n", path, buf);	

		

		return 0;
	}

}
