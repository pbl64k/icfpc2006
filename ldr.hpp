#ifndef R_UM_HDR_LDR

#define R_UM_HDR_LDR

#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>

#include <string>

#include "um.hpp"

#include "mem.hpp"
#include "end.hpp"

R_UM_NS_START

	class PageLoader {
		friend class Processor;
		friend class TestSuite;
		protected:
			static Word loadPage(MemMan *mm, const std::string &filename);
#ifdef R_UM_OPT_LITTLE_ENDIAN_HARDWARE
		private:
			static void fixEndianness(Word *block, Word size);
#endif
	};

R_UM_NS_END

#endif
