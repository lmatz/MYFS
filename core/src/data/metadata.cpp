#include <data/metadata.hpp>
#include <data/dir.hpp>
#include <op/functions.hpp>
#include <utils/log.hpp>

#include <sys/stat.h>

#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace myfs {

	GlobalMetadata::GlobalMetadata() {}

	int GlobalMetadata::initialize(const std::string &meta_filename, int cache_size, uint32_t num_bytes, uint32_t num_inodes) {
		if (this->init) {
			Log::log_msg("GlobalMetadata has already been initialized\n");
			return -1;
		}
		this->init = true;
		//this->path_to_ino_t.set_max_size(cache_size);
		this->free_bits_i = 0;
		this->free_bits_d = 0;
		fprintf(stderr, "meta_filename:%s with size:%d\n", meta_filename.c_str(), num_bytes);
		this->file.open(meta_filename, boost::iostreams::mapped_file::readwrite, num_bytes);
		fprintf(stderr, "boost open file: metadata\n");
		fprintf(stderr, "metadata file has size:%lu\n", this->file.size());
		fprintf(stderr, "myfs_inode size:%lu\n", sizeof(myfs_inode));

		determine_layout(num_bytes, num_inodes);
		
		fprintf(stderr, "num_bits_i = %d", this->num_bits_i);
		fprintf(stderr, "num_bits_d = %d", this->num_bits_d);

		const char *pic = "--inode bitmap-----data block bitmap-----inodes-----data blocks-----\n"
					      "|               |                     |          |                  |\n"
					      "|               |                     |          |                  |\n"
					      "|               |                     |          |                  |\n"
					      "--------------------------------------------------------------------\n";

		fprintf(stderr, pic);

		fprintf(stderr, "inode_bitmap starts from %d, end at %d\n", 0, this->num_bits_i-1);
		fprintf(stderr, "data_block_bitmap starts from %d, end at %d\n", this->num_bits_i, this->num_bits_i + this->num_bits_d - 1);
		
		char* data = this->file.data();
		memset(data, 0, this->num_bits_i/8);
		memset(data + this->num_bits_i/8, 0, this->num_bits_d/8);

		initialize_root_dir();

		return 0;
	}
	
	GlobalMetadata::~GlobalMetadata() {
		this->file.close();
	}

	int GlobalMetadata::make_directory(const char *path, mode_t mode) {
		int res = 0;
		myfs_ino_t ino;


		std::vector<std::string> strs;
		std::string path_str(path);
		boost::split(strs, path_str, boost::is_any_of("\t "));

		return 0;
	}

	int GlobalMetadata::change_mode(const char *path, mode_t mode) {
		int res = 0;
		myfs_ino_t ino;
		myfs_inode *inode = NULL;

		res = search_inode(&ino, path);
		if (res != 0) {
			Log::log_msg("GlobalMetadata::change_mode Search inode for path:%s fails. Path doesn't exist.\n", path);	
			return -1;
		}
		res = get_inode(ino, inode);
		if (res != 0) {
			return -1;	
		}
		
		char buf[10];
		strmode(inode->i_mode, buf);
		
		Log::log_msg("%s's original mode_t is %s\t", path, buf);
		inode->i_mode = mode;
		strmode(inode->i_mode, buf);
		Log::log_msg("%s's new mode_t is %s\n", path, buf);
		return 0;
	}

	int GlobalMetadata::change_owner(const char *path, uid_t uid, gid_t gid) {
		int res = 0;
		myfs_ino_t ino;
		myfs_inode *inode = NULL;

		res = search_inode(&ino, path);
		if (res != 0) {
			Log::log_msg("GlobalMetadata::change_owner Search inode for path:%s fails. Path doesn't exists.\n", path);
			return -1;
		}
		res = get_inode(ino, inode);
		if (res != 0) {
			return -1;
		}

		char buf[10];
		
		Log::log_msg("%s's original uid is %d gid is %d\t", inode->i_uid, inode->i_gid);
		inode->i_uid = uid;
		inode->i_gid = gid;
		Log::log_msg("%s's new uid is %d gid is %d\n", inode->i_uid, inode->i_gid);
		return 0;
	}

	int GlobalMetadata::check_access(const char *path, int mask, uid_t uid, gid_t gid) {
		int res = 0;
		myfs_ino_t ino;
		myfs_inode *inode = NULL;
		
		res = read_inode(path, &ino, inode);
		if (res != 0) {
			Log::log_msg("GlobalMetadata::check_access read inode for path:%s fails.\n");
			return res;
		}
		
		uid_t ino_uid = inode->i_uid;
		gid_t ino_gid = inode->i_gid;
		mode_t ino_mode = inode->i_mode;	

		res = check_permission(ino_uid, ino_gid, ino_mode, uid, gid, mask);	
		return res;
	}

	int GlobalMetadata::read_inode(const char *path, myfs_ino_t *ino_ret, myfs_inode *inode) {
		int res = 0;
		// first we check whether the inode associated with the path is in the cache
		//bool exist = path_to_ino_t.exists(std::string(path));
		myfs_ino_t ino;
		//if (exist) {
		//	ino_t = path_to_ino_t.get(std::string(path));
		//}
		//else {
			res = search_inode(&ino, path);
			if (res != 0) {
				Log::log_msg("GlobalMetadata::read_inode Search inode for path:%s fails. Path doesn't exist.\n", path);
				return res;
			}
		//	path_to_ino_t.put(path, ino_t);
		//}

		res = get_inode(ino, inode);
		if (res != 0) {
			return res;
		}
		*ino_ret = ino;

		return res;
	}

	int GlobalMetadata::write_inode(myfs_ino_t ino, myfs_inode *inode) {
		myfs_inode *inode_old = NULL;
		int res = get_inode(ino, inode);
		if (res != 0) {
			return res;
		}
		memcpy(inode_old, inode, inode_size);
		return 0;
	}


	int GlobalMetadata::search_inode(myfs_ino_t *ino_ret, const char *path) {
		int len = strlen(path);
		char *target_path = (char*)malloc((len+1)*sizeof(char));
		strcpy(target_path, path);
		// check whether it is root directory
		// if it is, we just return 1 because the inode number for root directory is 1
		if (strcmp(target_path,"/") == 0) {
			*ino_ret = 1;
			return 0;
		}

		// get each subdirectory name
		// all intermediate paths should be directory
		// the last one can be a directory or a file
		std::vector<char*> names;
		const char *sep = "/"; 
  		char *p;
  		p = strtok(target_path, sep);
  		while(p){
    		names.push_back(p);	
    		p = strtok(NULL, sep);
  		}

		// we start from root directory
		std::string current_dir = "/";
		myfs_ino_t ino = 1;
		myfs_inode *inode = NULL;
		int res = get_inode(ino, inode);
		if (res != 0) {
			return res;
		}

		for(int i=0; i<names.size(); i++) {
			// ino_t is current directory's inode number
			// numes[i] is the sub directory or file we want to look for
			// the returned ino_t is the sub directory or file's inode number
			Log::log_msg("try to find %s under %s", names[i], current_dir);
			res = find_file_in_dir(&ino, names[i]);
			// if there doesn't exist such a sub directory or file
			if (res != 0) {
				Log::log_msg("cannot find %s under directory %s\n", names[i], current_dir);
				return res;
			}
			// if there exists, we get the inode for that sub directory or file
			res = get_inode(ino, inode);
			if (res != 0) {
				return res;
			}
			// if this is not the last level, then it must be a directory
			if (i < names.size()-1) {
				// if it is not a directory, then we abort
				if (!S_ISDIR(inode->i_mode)) {
					Log::log_msg("supposed sub directory %s under directory %s is NOT a directory", names[i], current_dir);
					return -1;
				}
			}

			current_dir += std::string(names[i]) + "/";
		}

		*ino_ret = ino;
		Log::log_msg("we find %s. Its inode number is %lu", current_dir, *ino_ret);
		return 0;
	}

	int GlobalMetadata::find_file_in_dir(myfs_ino_t *ino_ret, const char *name) {
		int res = 0;
		myfs_inode *inode = NULL;
		res = get_inode(*ino_ret, inode);
		if (res != 0) {
			return res;
		}

		uint16_t i_blocks = inode->i_blocks;
		Log::log_msg("inode %lu has %u blocks", *ino_ret, i_blocks);
		//Dir dir(inode);

		return res;
	}

	/*
	int GlobalMetadata::convert_data_block_to_subdirs(std::vector<std::string> &subdirs, char *data, int len) {
		DirEntry *dir_entry = (DirEntry*)data;

		int offset = 0;
		
		while(offset < len) {
			myfs_ino_t i_num = dir_entry->i_num;
			int rec_len = dir_entry->rec_len;
			int str_len = dir_entry->str_len;
			char *sub_name = dir_entry->str;
			std::string s(sub_name, str_len);
			subdirs.push_back(s);
			data += sizeof(myfs_ino_t);
			data += 2*sizeof(int);
			data += rec_len;
			dir_entry = (DirEntry*)data;
			offset += sizeof(myfs_ino_t);
			offset += sizeof(int);
			offset += rec_len;
		}
	}
	*/


	int GlobalMetadata::initialize_root_dir() {
		// the first inode is allocated for root dir
		char* i_bitmap = inode_bitmap();
		i_bitmap[0] = 0x1;
		// the first data block is allocated for root dir
		char* d_bitmap = data_block_bitmap();
		d_bitmap[0] = 0x1;

		myfs_ino_t root_ino = 1;
		myfs_inode *root_inode = NULL;
		int res = get_inode(root_ino, root_inode);
		if (res != 0) {
			Log::log_msg("GlobalMetadata::initialize_root_dir cannot get root inode with index 1, Unbelievable!\n");
			return res;
		}

		//root_dir.reset(new Dir(root_inode));

		return 0;
	}

	char* GlobalMetadata::inode_bitmap() {
		return this->file.data();
	}

	char* GlobalMetadata::data_block_bitmap() {
		char* data = this->file.data();
		return data + num_bits_i / 8;
	}

	int GlobalMetadata::get_inode(const myfs_ino_t index, myfs_inode *res) {
		if (index > this->num_bits_i) {
			Log::log_msg("GlobalMetadata::get_inode inode number: %u and number of inodes: %u. Out of range", index, this->num_bits_i);
			return -1;
		}
		char* data = this->file.data();
		uint32_t offset = 0;
		offset += num_bits_i / 8;
		offset += num_bits_d / 8;
		offset += inode_size * (index-1);

		data += offset;
		res = (myfs_inode*)data;
		return 0;
	}

	int GlobalMetadata::get_data_block(uint32_t index, char *res) {
		if (index >= this->num_bits_d) {
			Log::log_msg("GlobalMetadata::get_data_block index: %u and number of inodes: %u. Out of range", index, this->num_bits_i);
			return -1;
		}
		char* data = this->file.data();
		uint32_t offset = 0;
		offset += num_bits_i / 8;
		offset += num_bits_d / 8;
		offset += inode_size * num_bits_i;
		offset += data_block_size * index;
			
		data += offset;
		Log::log_msg("GlobalMetadata::get_data_block index: %u and the resulting offset: %u.\n", index, offset);
		res = data;
		return 0;
	}

	void GlobalMetadata::determine_layout(const uint32_t num_bytes, const uint32_t num_inodes) {
		// we have a 128 bytes struct for each inode struct
		// we have 512 bytes for each data block
		// Since we always store content of a file in remote workers
		// we don't have much to put into data block
		// The data block here stores two things
		// 1 information about directories, namely, names of files in the directory.
		// 2 data block pointers
		// So no need for too much space allocated for data blocks
		// because the actual data of a file is stored on workers
		// here we check how much percentage of space will be allocated for 'num_inodes' inodes
		
		this->num_bits_i = num_inodes;
		this->free_bits_i = this->num_bits_i;

		// Since one byte = 8 bits, num_bytes_inode_bitmap is the number of bytes we need to represent bitmap
		uint32_t num_bytes_inode_bitmap = num_inodes / 8;
		// number of bytes for storing actual inode struct
		uint32_t num_bytes_inodes = inode_size * num_inodes;
		// number of bytes for storing both the inode structs and inode bitmap
		uint32_t num_bytes_all_i = num_bytes_inode_bitmap + num_bytes_inodes;

		fprintf(stderr, "all bytes allocated for inodes: %d\n", num_bytes_all_i);
		fprintf(stderr, "it takes %.1f percentages of all space\n", (double)num_bytes_all_i / num_bytes);

		// left space for storing data block bitmap and actual data blocks
		uint32_t free_bytes = num_bytes - num_bytes_all_i;
		
		size_t num_blocks = 1;
		size_t num_bytes_all_d = num_blocks + data_block_size * num_blocks / 8;
		while(num_bytes_all_d <= free_bytes) {
			num_blocks *= 2;
			num_bytes_all_d = num_blocks + data_block_size * num_blocks / 8;
		}
		num_blocks -= 1;
		num_bytes_all_d = num_blocks + data_block_size * num_blocks / 8;

		this->num_bits_d = num_blocks;
		this->free_bits_d = this->num_bits_d;

		fprintf(stderr, "%lu data blocks will be allocated\n", num_blocks);
		fprintf(stderr, "all bytes allocated for data blocks: %lu\n", num_bytes_all_d);
		fprintf(stderr, "it takes %.1f percentages of all space\n", (double)num_bytes_all_d / num_bytes);
		
		free_bytes -= num_bytes_all_d;
		fprintf(stderr, "Unused bytes: %d\n", free_bytes);
		fprintf(stderr, "it takes %.1f percentages of all space\n", (double)free_bytes / num_bytes_all_d);
	}


	int GlobalMetadata::allocate_new_inode(myfs_ino_t *idx) {
		char *bitmap = inode_bitmap();
		for(size_t i = 0 ; i < num_bits_i / 8 ; i++) {
			uint8_t bit = 1;
			*idx = 1;
			while(bit <= 256 && (bitmap[i] & bit) != 0) {
				bit = bit << 1;
				*idx += 1;
			}
			if (bit != 512) {
				*idx = 8 * i + *idx;
				this->free_bits_i--;
				return 0;
			}
		}
		return -1;
	}

	int GlobalMetadata::allocate_new_data_block(uint32_t *idx) {
		char *bitmap = data_block_bitmap();
		for(size_t i = 0 ; i < num_bits_d / 8 ; i++) {
			uint8_t bit = 1;
			*idx = 1;
			while(bit <= 256 && (bitmap[i] & bit) != 0) {
				bit = bit << 1;
				*idx += 1;
			}
			if (bit != 512) {
				*idx = 8 * i + *idx;
				this->free_bits_d--;
				return 0;
			}
		}
		return -1;
	}


	// this function should only be called when we are sure that this file exists.
	int GlobalMetadata::check_permission(uid_t ino_uid, gid_t ino_gid, mode_t mode, uid_t uid, gid_t gid, int mask) {
		int res = 0;
		if (mask & F_OK) {
			return 0;
		}

		// if the user want to check whether he has the read access
		if (mask & R_OK) {
			// if the user belongs to "owner"
			if (ino_uid == uid) {
				// if user cannot read the file
				if (!(mode & S_IRUSR)) {
					res = -1;
				}
			}
			// if the user belongs to "group"
			else if (ino_gid == gid) {
				// if group cannot read the file
				if (!(mode & S_IRGRP)) {
					res = -1;
				}
			}
			// if the user belongs to "other"
			else {
				// if other cannot read the file
				if (!(mode & S_IROTH)) {
					res = -1;
				}
			}
		}

		// if the user want to check whether he has the write access
		if (mask & W_OK) {
			// if the user belongs to "owner"
			if (ino_uid == uid) {
				// if user cannot write the file
				if (!(mode & S_IWUSR)) {
					res = -1;
				}
			}
			// if the user belongs to "group"
			else if (ino_gid == gid) {
				// if group cannot write the file
				if (!(mode & S_IWGRP)) {
					res = -1;
				}
			}
			// if the user belongs to "other" 
			else {
				// if group cannot write the file
				if (!(mode & S_IWOTH)) {
					res = -1;
				}
			}
		}

		// if the user want to check whether he has the executable access
		if (mask & X_OK) {
			// if the user belongs to "owner"
			if (ino_uid == uid) {
				// if user cannot execute the file
				if (!(mode & S_IXUSR)) {
					res = -1;
				}
			}
			// if the user belongs to "group"
			else if (ino_gid == gid) {
				// if group cannot execute the file
				if (!(mode & S_IXGRP)) {
					res = -1;
				}
			}
			// if the user belongs to "other"
			else {
				// if other cannot execute the file
				if (!(mode & S_IXOTH)) {
					res = -1;
				}
			}
		}
		return res;
	}
}
