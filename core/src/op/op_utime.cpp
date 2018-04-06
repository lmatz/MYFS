#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/functions.hpp>
#include <op/op_myfs.hpp>

namespace myfs {

	int op_utime(const char *path, struct utimbuf *buf) {
		Log::log_msg("op_utime should be deprecated. But it gets called!");
		GlobalMetadata *meta = MYFS_METADATA;

		int res;
		myfs_ino_t ino_t;
		myfs_inode *inode;

		res = check_path(path);
		if (res != 0) {
			Log::log_error("op_utime check_path fails.");
			return res;
		}


		res = meta->read_inode(path, &ino_t, inode);
		if (res!=0) {
			Log::log_error("op_utime read_inode fails.");
			return res;
		}

		inode->i_ctime = buf->actime;
		inode->i_mtime = buf->modtime;

		Log::log_msg("op_utime change inode %d access time to %d and modification time to %d\n", ino_t, inode->i_ctime, inode->i_mtime);
		
		Log::log_msg("op_utime success.\n");
		return 0;
	}

}
