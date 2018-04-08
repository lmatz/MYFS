#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>

#include <time.h>


namespace myfs {

	int op_utimens(const char *path, const struct timespec tv[2]) {
		struct tm buf;
		char *access_time = asctime(gmtime(&tv[0].tv_sec));
		char *modi_time = asctime(gmtime(&tv[1].tv_sec));
		
		Log::log_msg("op_utimens path:%s, access time:%s, modification time:%s\n", path, access_time, modi_time);
		int res = 0;



		return 0;
	}

}
