#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>




namespace myfs {
	
	int op_fgetattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi) {
		Log::log_msg("op_fgetattr path:%s\n");	
		int res = 0;

		return 0;
	}

}
