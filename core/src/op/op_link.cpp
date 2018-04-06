#include <utils/log.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_link(const char *from, const char *to) {
		Log::log_msg("op_link from:%s to:%s", from, to);
		return 0;
	}
	

}
