#include <utils/log.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_rename(const char *from, const char *to) {
		Log::log_msg("op_rename from:%s to:%s", from, to);
		return 0;
	}

}
