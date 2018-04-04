#include <stdint.h>

#include <utils/type.hpp>

namespace myfs {
	
	struct DirEntry {
		myfs_ino_t i_num;
		int        rec_len;
		int        str_len;
		char       str[1];
	};


}
