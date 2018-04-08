#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>

namespace myfs {

	int op_open(const char *path, fuse_file_info *fi) {
		Log::log_msg("op_open path:%s\n", path);

		return 0;
	}

}
