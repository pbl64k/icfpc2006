#ifndef R_UM_HDR_CON

#define R_UM_HDR_CON

#include <termios.h>
#include <unistd.h>

#include <cstdio>

#include "um.hpp"

R_UM_NS_START

	class Console {
		friend class Processor;
		friend class TestSuite;
		protected:
			static void init();
			static void release();
			static void putByte(Word character);
			static Word getByte();
		private:
			static void reconfigTerm();
			static void restoreTerm();
			static void startSnapshot();
			static void stopSnapshot();
			static void addToSnapshot(Byte character);
			static bool isSnapshotValid();

#ifdef R_UM_OPT_LINUX_NOLINEBUF_STDIN
			static termios termiosOld_;
#endif

#ifdef R_UM_OPT_STDOUT_SNAPSHOT
			static FILE *snapshot_;
#endif
	};

R_UM_NS_END

#endif
