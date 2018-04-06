#include <utils/log.hpp>
#include <op/functions.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_mkdir(const char *path, mode_t mode) {
		char buf[10];
		strmode(mode, buf);
		Log::log_msg("op_mkdir path:%s, mode:%s\n", path, buf);

		int res;
		

		
		return 0;
	}

}
