#pragma once
#include <sys/stat.h>

namespace myfs {

	int check_path(const char *path);

	// print mode_t nicely
	void strmode(mode_t mode, char *buf);

}
