#include <utils/config.hpp>

#include <string>

namespace myfs {


	void Config::putString(const std::string &key, const std::string &val) {
		this->config_map[key] = val;
	}

	void Config::putInteger(const std::string &key, const size_t &val) {
		this->config_map[key] = std::to_string(val);
	}

	std::string Config::getString(const std::string &key) {
		return this->config_map[key];
	}

	std::string Config::getStringDefault(const std::string &key, const std::string &default_val) {
		if (this->config_map.find(key) != this->config_map.end()) {
			return this->config_map[key];
		}
		return default_val;
	}

}
