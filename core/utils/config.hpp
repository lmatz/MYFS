#pragma once
#include <stdint.h>

#include <unordered_map>

namespace myfs {

	class Config {
	public:

		Config &get_instance() {
			static Config config;
			return config;
		}


		void putString(const std::string &key, const std::string &val);

		void putInteger(const std::string &key, const size_t &val);

		std::string getString(const std::string &key);

		std::string getStringDefault(const std::string &key, const std::string &default_val);


	private:

		Config();

		std::unordered_map<std::string, std::string> config_map;

	};


}
