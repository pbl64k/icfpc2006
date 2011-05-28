#ifndef R_UM_HPP_UM

#define R_UM_HPP_UM

#define R_UM_OPT_LITTLE_ENDIAN_HARDWARE
#undef R_UM_OPT_ENFORCE_LOWLEV_VALIDITY
#undef R_UM_OPT_SELFTEST
#undef R_UM_OPT_STDOUT_SNAPSHOT
#undef R_UM_OPT_LINUX_NOLINEBUF_STDIN
#undef R_UM_OPT_ENABLE_LOGGING
#undef R_UM_OPT_PRC_DEBUG
#undef R_UM_OPT_MEM_DEBUG
#undef R_UM_OPT_BLK_DEBUG
#undef R_UM_OPT_RAW_DEBUG

#include <cassert>

#define R_UM_NS_START namespace Ranselett { \
	namespace Icfpc { \
		namespace UniversalMachine {

#define R_UM_NS_END } \
		} \
	}

R_UM_NS_START

	typedef unsigned char Byte;

	typedef unsigned int Word;

	static const Byte wordSize = 4;

R_UM_NS_END

#endif
