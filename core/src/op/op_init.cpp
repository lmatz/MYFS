#include <utils/log.hpp>
#include <myfs/myfs.hpp>
#include <utils/config.hpp>
#include <utils/log.hpp>

namespace myfs {

	void *op_init (struct fuse_conn_info *conn) {
		Log::log_msg("op_init");
		conn->max_write = Config::max_write;
		return (void*)conn;
	}

}
