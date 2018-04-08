#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>




namespace myfs {

	int op_statfs(const char *path, struct statvfs *stfs) {
		Log::log_msg("op_statfs path:%s\n", path);	
		int res = 0;	


		return 0;
	}

}
