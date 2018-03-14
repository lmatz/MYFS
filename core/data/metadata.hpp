#pragma once
#include <unordered_map>

#include <utils/lru_cache.hpp>
#include <utils/type.hpp>


namespace myfs {

	class GlobalMetadata {
	public:
		
		static GlobalMetadata& get_instance() {
			static GlobalMetadata singleton;
			return singleton;
		}

		int read_inode(const char *path, myfs_ino_t *, myfs_inode *);

		int write_inode(myfs_ino_t, myfs_inode *);

	private:
		GlobalMetadata(): path_to_ino_t(1000) {}

		~GlobalMetadata() {}

		std::unordered_map<myfs_ino_t, myfs_inode*> ino_t_to_inode;

		lru_cache<std::string, myfs_ino_t> path_to_ino_t;

	};

}
