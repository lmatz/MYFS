#include <stdlib.h>

#include <utils/log.hpp>
#include <utils/macros.hpp>
#include <op/op_myfs.hpp>
#include <op/functions.hpp>


namespace myfs {

	int op_getattr(const char *path, struct stat *stbuf) {
		Log::log_msg("op_getattr path:%s\n", path);	
		GlobalMetadata *meta = MYFS_METADATA;
		int res = 0;

		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
		myfs_ino_t ino;
		myfs_inode *inode;

		// check whether the path is valid
		res = check_path(path);
		if (res != 0) {
			Log::log_error("op_getattr check_path fails.");
			return res;	
		}

		// read the inode associated with the path
		// and fill the information required to the stbuf
		res = meta->read_inode(path, &ino, inode);
		if (res != 0) {
			Log::log_error("op_getattr read_inode fails.");
			return res;
		}

		res = do_fill_stat(stbuf, ino, inode);

		Log::log_msg("op_getattr success.\n");
		return 0;
	} 	

}
