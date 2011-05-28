#include "log.hpp"

R_UM_NS_START

	Logger::Logger(const std::string &filename) {
#ifdef R_UM_OPT_ENABLE_LOGGING
		assert(log_ = std::fopen(filename.c_str(), "w"));
#else
		std::printf(
				"Cannot start logging to %s, logging disabled at compile-time\n",
				filename.c_str());
		assert(false);
#endif
	}

	Logger::~Logger() {
#ifdef R_UM_OPT_ENABLE_LOGGING
		assert(! std::fclose(log_));
#else
		assert(false);
#endif
	}

	void Logger::log(const std::string &msg) {
#ifdef R_UM_OPT_ENABLE_LOGGING
		assert(std::fputs(msg.c_str(), log_) != EOF);
#else
		std::printf(
				"Cannot write %s to log, logging disabled at compile-time\n",
				msg.c_str());
		assert(false);
#endif
	}

R_UM_NS_END
