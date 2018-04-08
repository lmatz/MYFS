#pragma once
#include <sys/stat.h>

namespace myfs {

	// check whether the 'path' is valid or not (Not checking existence)
	int check_path(const char *path);

	// print mode_t nicely
	void strmode(mode_t mode, char *buf);

	// print masj in access() nicely
	void straccess(int mask, char *buf);

}
