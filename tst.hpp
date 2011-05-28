#ifndef R_UM_HDR_TST

#define R_UM_HDR_TST

#include "um.hpp"

#ifdef R_UM_OPT_SELFTEST

#include <cstdio>

#include <iostream>
#include <ostream>
#include <string>

#include "raw.hpp"
#include "blk.hpp"
#include "mem.hpp"
#include "ldr.hpp"
#include "reg.hpp"
#include "con.hpp"

R_UM_NS_START

	class TestSuite {
		public:
			static void run(const std::string &myself);
		private:
			static void testSanity(Word invalidByte);
			static void testRawBlock();
			static void testLazyBlock();
			static void testMemMan();
			static void testPageLoader(const std::string &myself);
			static void testRegisterBlock();
			static void testConsole();
	};

R_UM_NS_END

#endif

#endif
