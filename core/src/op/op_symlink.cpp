#include <utils/log.hpp>
#include <op/op_myfs.hpp>


namespace myfs {

	int op_symlink(const char *to, const char *from) {
		Log::log_msg("op_symlink to:%s from:%s", to, from);
		return 0;
	}

}
