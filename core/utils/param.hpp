#pragma once

#define FUSE_USE_VERSION 26

#include <limits.h>
#include <stdint.h>
#include <stdio.h>

namespace myfs {

	struct myfs_state {
		unsigned char debug;
		uint64_t theta;
		FILE *logfile;
		char *log_filename;
		char *meta_dir;
	};

}

#define MYFS_DATA ((struct myfs_state *) fuse_get_context()->private_data)
