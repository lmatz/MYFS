#include <string.h>
#include <sys/stat.h>

#include <data/dir.hpp>
#include <utils/log.hpp>

#include <string>

namespace myfs {

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

	Dir::Dir(myfs_ino_t ino, myfs_inode *inode) : dir_inode(inode), dir_ino(ino) {
		//init();	
	}

	void Dir::init() {
		GlobalMetadata &metadata = GlobalMetadata::get_instance();
		int res = 0;
		// assert the inode represents a directory
		assert(S_ISDIR(this->dir_inode->i_mode));
		assert(this->dir_inode->i_blocks <= NUM_BLOCKS);
		
		// we want to explore the all the data blocks of this directory to find the maximum number of empty space for each data blocks
		// when we want to insert a new sub directory entry
		// we can try to best fit it
		//
		// if num of data blocks is less than NUM_BLOCKS
		// then all of these data blocks are served as actual data blocks
		// if num of data blocks is equal to NUM_BLOCKS, then the first NUM_BLOCKS-1 data blocks put actual data while the last data block will store indirect data block pointers
		if (this->dir_inode->i_blocks < NUM_BLOCKS) {
			for(uint32_t i = 0; i < this->dir_inode->i_blocks; i++) {
				char *data_block = NULL;
				res = metadata.get_data_block(this->dir_inode->i_block[i], data_block);	
				if (res != 0) {
					Log::log_msg("Dir::init invalid data block index: %lu\n", this->dir_inode->i_block[i]);
					return;
				}
				std::unordered_map<std::string, myfs_ino_t> temp_name_to_ino; 
				res = get_dir_names_from_data_block(data_block, temp_name_to_ino);
				if (res != 0) {
					Log::log_msg("Dir::init invalid data block for get sub directory names index: %lu\n", this->dir_inode->i_block[i]);
					return;
				}
				// we put all the sub directories we find in the 'name_to_ino'
				for(auto& p : temp_name_to_ino) {
					this->name_to_ino[p.first] = p.second;
				}
			}
		}
		else {

		}
	}

	// return 0 indicates that we successfully add the block and the block stores actual data
	// return 1 indicates that we successfully add the block and the block stores pointers to actual data block
	int Dir::add_new_data_block(uint32_t block_idx) {
		GlobalMetadata &metadata = GlobalMetadata::get_instance();
		uint32_t blocks = this->dir_inode->i_blocks;
		if (blocks < (NUM_BLOCKS-1)) {
			this->dir_inode->i_blocks++;
			this->dir_inode->i_block[blocks] = block_idx;
			Log::log_msg("Dir::add_new_data_block with idx:%lu and has %lu blocks now\n", block_idx, this->dir_inode->i_blocks);
			return 0;
		}
		else if (blocks == (NUM_BLOCKS-1)) {
			this->dir_inode->i_blocks++;
			this->dir_inode->i_block[blocks] = block_idx;
			char *data_block = NULL;
			int res = metadata.get_data_block(block_idx, data_block);
			Log::log_msg("Dir::add_new_data_block with idx:%lu and has %lu blocks now\n", block_idx, this->dir_inode->i_blocks);
			return 1;
		}
		else if (blocks >= (NUM_BLOCKS)) {
			this->dir_inode->i_blocks++;
			// get the index for the last data block which stores pointers
			uint32_t indirct_block_idx = this->dir_inode->i_block[NUM_BLOCKS-1];
			char *data_block = NULL;
			int res = metadata.get_data_block(indirct_block_idx, data_block);
			uint32_t *pointer = (uint32_t*) data_block;
			// 
			uint32_t num_pointers = this->dir_inode->i_blocks - NUM_BLOCKS -1;
			pointer += num_pointers;
			*pointer = block_idx;
			Log::log_msg("Dir::add_new_data_block with idx:%lu and has %lu blocks now\n", block_idx, this->dir_inode->i_blocks);
			return 0;
		}
		else {
			assert(false && "Dir::add_new_data_block, This line should not get called");
			return -1;
		}
		return 0;
	}

	int Dir::insert_subdir(myfs_ino_t ino, const char *name) {
		GlobalMetadata &metadata = GlobalMetadata::get_instance();

		// 1 is for the '\0' char
		uint32_t str_len = strlen(name) + 1;
		uint32_t total_bytes_needed = sizeof(myfs_ino_t) + 2*sizeof(int) + str_len;

		int res = 0;
		// we first iterate over the first level data block which actually stores data
		uint32_t num_first_level_data_blocks = min(this->dir_inode->i_blocks, NUM_BLOCKS-1);
		for(uint32_t i = 0; i < this->dir_inode->i_blocks; i++) {
			uint32_t space = total_bytes_needed;
			char *loc = NULL;
			DirEntry *prev_dir_entry = NULL;
			DirEntry *next_dir_entry = NULL;
			res = find_space_in_data_block(&space, loc, prev_dir_entry, next_dir_entry, this->dir_inode->i_block[i]);
			if (res == 0) {
				DirEntry *dir_entry = (DirEntry*) loc;
				dir_entry->i_num = ino;
				dir_entry->rec_len = space - sizeof(myfs_ino_t) - 2*sizeof(int);
				dir_entry->str_len = str_len;
				strcpy(dir_entry->str, name);
				// if the space is not at the first dir entry of the data block
				if (prev_dir_entry !=  NULL) {
					prev_dir_entry->rec_len = prev_dir_entry->str_len;
				}
				return 0;
			}
		}

		// if we has not enough first level data block, then we just allocate a new one
		if (num_first_level_data_blocks < NUM_BLOCKS-1) {
			uint32_t idx;
			res = metadata.allocate_new_data_block(&idx);	
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 1 cannot allocate new data block\n");
				return -1;
			}
			res = add_new_data_block(idx);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 1 cannot add new data block into current dir\n");
				return -1;
			}
			char *data_block = NULL;
			res = metadata.get_data_block(idx, data_block);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 1 cannot get data block with index:%lu\n", idx);
				return -1;
			}
			DirEntry *dir_entry = (DirEntry*) data_block;
			dir_entry->i_num = ino;
			dir_entry->rec_len = BLOCK_SIZE - sizeof(myfs_ino_t) - sizeof(int);
			dir_entry->str_len = str_len;
			strcpy(dir_entry->str, name);
			return 0;
		}
		// if we already had NUM_BLOCKS-1 data block, then we need to allocate two new data blocks
		// one for storing pointers to data blocks
		// one for storing acutal data
		else if (num_first_level_data_blocks == (NUM_BLOCKS-1)) {
			// first allocate the data block for storing pointers
			uint32_t idx;
			res = metadata.allocate_new_data_block(&idx);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 2 cannot allocate new data block\n");
				return -1;
			}	
			res = add_new_data_block(idx);
			if (res != 1) {
				Log::log_msg("Dir::insert_subdir 2 cannot add new data block for storing pointers\n");
				return -1;
			}
			char *data_block = NULL;
			res = metadata.get_data_block(idx, data_block);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 2 cannot get data block with index:%lu\n", idx);
				return -1;
			}
			// then allocate the data block for storing acutal data
			res = metadata.allocate_new_data_block(&idx);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 3 cannot allocate new data block\n");
				return -1;
			}
			res = add_new_data_block(idx);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 3 cannot add new data block for storing actual data\n");
				return -1;
			}
			char *data_block2 = NULL;
			res = metadata.get_data_block(idx, data_block2);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 3 cannot get data block with index:%lu\n", idx);
			}
			
			uint32_t *pointer1 = (uint32_t*) data_block;
			*pointer1 = 1;
			pointer1++;
			*pointer1 = idx;
			DirEntry *dir_entry = (DirEntry*) data_block2;
			dir_entry->i_num = ino;
			dir_entry->rec_len = BLOCK_SIZE - sizeof(myfs_ino_t) - sizeof(int);
			dir_entry->str_len = strlen(name) + 1;
			strcpy(dir_entry->str, name);
			return 0;	
		}
		// if we already have more than `NUM_BLOCKS` data blocks
		// then we first need to iterate over all these data blocks pointed by pointers
		// if all these actual data blocks don't have enough space to put this new dir entry
		// we allocate a new data block
		else if (this->dir_inode->i_blocks > NUM_BLOCKS) {
			// this is the last data block which stores pointers
			uint32_t idx = this->dir_inode->i_block[(size_t)NUM_BLOCKS-1];
			char *data_block = NULL;
			res = metadata.get_data_block(idx, data_block);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 4 cannot get data block with index:%lu\n", idx);
				return -1;
			}
			uint32_t *pointer = (uint32_t*) data_block;
			// get num of pointers on this data block
			uint32_t num = this->dir_inode->i_blocks - NUM_BLOCKS;
			for(uint32_t i = 0; i < num; i++) {
				idx = *(pointer+i);
				res = metadata.get_data_block(idx, data_block);
				if (res != 0) {
					Log::log_msg("Dir::insert_subdir 4 cannot get %luth data block with index:%lu\n", i, idx);
					return -1;
				}
				uint32_t space = total_bytes_needed;
				char *loc = NULL;
				DirEntry *prev_dir_entry = NULL;
				DirEntry *next_dir_entry = NULL;
				res = find_space_in_data_block(&space, loc, prev_dir_entry, next_dir_entry, idx);
				if (res == 0) {
					DirEntry *dir_entry = (DirEntry*) loc;
					dir_entry->i_num = ino;
					dir_entry->rec_len = space - sizeof(myfs_ino_t) - 2*sizeof(int);
					dir_entry->str_len = str_len;
					strcpy(dir_entry->str, name);
					if (prev_dir_entry != NULL) {
						prev_dir_entry->rec_len = str_len;
					}
					return 0;
				}
			}
			// if we get here, it means existing data blocks in current diretory have no free space for us
			// so we need to allocate a new data block
			res = metadata.allocate_new_data_block(&idx);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 5 cannot allocate a new data block\n");
				return -1;
			}
			res = add_new_data_block(idx);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 5 cannot add new data block for storing actual data\n");
				return -1;
			}
			// write the new block index into the pointer data block
			*(pointer + num) = idx;	
			res = metadata.get_data_block(idx, data_block);
			if (res != 0) {
				Log::log_msg("Dir::insert_subdir 5 cannot get data block with index:%lu\n", idx);
				return -1;
			}
			DirEntry *dir_entry = (DirEntry*) data_block;
			dir_entry->i_num = ino;
			dir_entry->rec_len = BLOCK_SIZE - sizeof(myfs_ino_t) - 2*sizeof(int);
			dir_entry->str_len = str_len;
			strcpy(dir_entry->str, name);
		}
		
	}

	int Dir::get_dir_names_from_data_block(char *data_block, std::unordered_map<std::string, myfs_ino_t>& res_map) {
		DirEntry *dir_entry = (DirEntry*)data_block;
		int total_len = BLOCK_SIZE;
		int offset = 0;

		while(offset < total_len) {
			myfs_ino_t ino = dir_entry->i_num;
			int rec_len = dir_entry->rec_len;
			int str_len = dir_entry->str_len;
			std::string sub_name(dir_entry->str, str_len);
			res_map[sub_name] = ino;

			offset += sizeof(myfs_ino_t) + 2 * sizeof(int) + rec_len;
			dir_entry = (DirEntry*)(data_block + offset);
		}
		return 0;	
	}

	// space is number of bytes requested by the caller and we will also return the acutal bytes availble to caller, it must be greater or equal to the input number
	// loc is the start of free space
	// previous dir entry, if any
	// next dir entry (TODO)
	// block_idx is the data block index
	// return 0 means we find the request space
	// return -1 means there isn't enough free space
	int Dir::find_space_in_data_block(uint32_t *space, char *loc, DirEntry *prev_dir_entry, DirEntry *next_dir_entry, uint32_t block_idx) {
		GlobalMetadata &metadata = GlobalMetadata::get_instance();	
		char *data_block = NULL;
		int res = metadata.get_data_block(block_idx, data_block);
		DirEntry *dir_entry = (DirEntry *) data_block;
		int offset = 0;
		while(offset < BLOCK_SIZE) {
			dir_entry = (DirEntry*) (data_block+offset);

			offset += sizeof(myfs_ino_t) + 2*sizeof(int) + dir_entry->str_len;

			uint32_t free_till_next_one = dir_entry->rec_len - dir_entry->str_len ;
			// we have enough free space between the current dir entry and its next dir entry
			// then we put the dir entry into the middle of these two
			// we need to modify the current dir entry's rec_len method
			if (*space <= free_till_next_one) {
				prev_dir_entry = dir_entry;	
				*space = free_till_next_one;
				loc = ((char*)dir_entry) + sizeof(myfs_ino_t) + 2*sizeof(int) + dir_entry->str_len;
				return 0;
			}
		}
		return -1;
	}


}
