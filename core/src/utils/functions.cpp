#include <utils/type.hpp>
#include <utils/functions.hpp>

#include <stdio.h>


namespace myfs {

	// return 0 means success
	// otherwise, master and worker should abort instead of calling fuse_main
	int check() {
		int res = 0;
		int size = sizeof(myfs_inode);
		fprintf(stderr, "Size of struct myfs_inode is %d\n", size);
		if (size <= 128) {
			fprintf(stderr, "But we expect size of struct myfs_inode is <= 128 bytes\n");
			return -1;
		}

		return 0;	
	}

}
