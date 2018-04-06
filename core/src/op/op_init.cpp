#include <data/metadata.hpp>
#include <op/op_myfs.hpp>
#include <utils/log.hpp>
#include <utils/config.hpp>
#include <utils/log.hpp>

#include <iostream>
#include <fstream>

namespace myfs {

	void *op_init (struct fuse_conn_info *conn) {
		return MYFS_DATA; 
	}

}
