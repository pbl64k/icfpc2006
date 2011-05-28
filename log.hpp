#ifndef R_UM_HDR_LOG

#define R_UM_HDR_LOG

#include <cstdio>

#include <string>

#include "um.hpp"

R_UM_NS_START

	class Logger {
		friend class TestSuite;
		friend class Processor;
		friend class MemMan;
		friend class LazyBlock;
		friend class RawBlock;
		protected:
			Logger(const std::string &filename);
			~Logger();
			void log(const std::string &msg);
		private:
			FILE *log_;
	};

R_UM_NS_END

#endif
