#include <errno.h>
#include <stdlib.h>

#include <op/functions.hpp>

#include <utils/log.hpp>
#include <utils/type.hpp>


namespace myfs {

	// return 0 means path is valid
	int check_path(const char *path) {
		char *path_dup = strdup(path);
		char *basename_path;
		basename_path = strrchr(path_dup, '/');
		if (basename_path == NULL) {
			Log::log_msg("this should mot happen %s", path);
			return -ENOENT;
		}
		basename_path++;
		if (strlen(basename_path) > 255) {
			Log::log_msg("basename exce3eds 255 characters %s", path);
			return -ENAMETOOLONG;
		}
		free(path_dup);
		return 0;
	}

	void strmode(mode_t mode, char *buf) {
  		const char chars[] = "rwxrwxrwx";
  		for (size_t i = 0; i < 9; i++) {
    		buf[i] = (mode & (1 << (8-i))) ? chars[i] : '-';
 	 	}
  		buf[9] = '\0';
	}


}
