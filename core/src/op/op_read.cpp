#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>

namespace myfs {

	int op_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi) {
		Log::log_msg("op_read path:%s, size:%lu, offset:%lu\n");
		GlobalMetadata *meta = MYFS_METADATA;
		int res = 0;

		myfs_ino_t ino;
		myfs_inode *inode;
		
		// check whether the path is valid
		res = check_path(path);
		if (res != 0) {
			Log::log_error("op_read check_path fails.");
			return res;
		}


		// read the inode associated with the path
		res = meta->read_inode(path, &ino, inode);
		if (res != 0) {
			Log::log_error("op_read read_inode fails.");
			return res;
		}

		

	}

}
