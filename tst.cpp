#include "tst.hpp"

#ifdef R_UM_OPT_SELFTEST

R_UM_NS_START

	static const Word byteSizeSanityCheck = 0x100;

	void TestSuite::run(const std::string &myself) {
		std::cout << "A blatantly superficial UniversalMachine test suite"
				<< std::endl << std::endl;

		testSanity(byteSizeSanityCheck);
		testConsole();
		testRawBlock();
		testLazyBlock();
		testMemMan();
		testPageLoader(myself);
		testRegisterBlock();

		std::cout << "[V] all tests ok." << std::endl << std::endl;
		std::cout << "[!] (don't forget to run with --sandmark and --stress"
				<< std::endl << "    to make sure it really works)"
				<< std::endl;
	}

	void TestSuite::testSanity(Word invalidByte) {
		std::cout << "sanity check..." << std::endl;
		
		std::cout << "\t[!] there's nothing we can do " <<
				"if your char is non-8 bit" << std::endl;

		assert(static_cast<Byte>(invalidByte) != invalidByte);

		std::cout << "\tByte size ok. (8 bits)" << std::endl;

		std::cout << "\t[!] if your unsigned int isn't 4 chars long, "
				<< "you can try to" << std::endl
				<< "\t    redefine Word in um.hpp using a different type"
				<< std::endl;

		assert(sizeof(Word) == wordSize);

		std::cout << "\tWord size ok. (" << (sizeof(Word) * 8) << " bits)"
				<< std::endl;

		std::cout << "[V] UniversalMachine sanity check ok."
				<< std::endl << std::endl;
	}

	void TestSuite::testRawBlock() {
		std::cout << "testing raw block..." << std::endl;

		static const Word testSize = 4;

		RawBlock *blk1 = new RawBlock(testSize);

		assert(blk1);

		(*blk1)[0] = 111111;
		(*blk1)[1] = 222222;
		(*blk1)[2] = 333333;
		(*blk1)[3] = 444444;

		assert((*blk1)[0] == 111111);
		assert((*blk1)[1] == 222222);
		assert((*blk1)[2] == 333333);
		assert((*blk1)[3] == 444444);

		RawBlock *blk2 = new RawBlock(*blk1);

		assert((*blk2)[0] == 111111);
		assert((*blk2)[1] == 222222);
		assert((*blk2)[2] == 333333);
		assert((*blk2)[3] == 444444);

		(*blk2)[3] = 111111;
		(*blk2)[2] = 222222;
		(*blk2)[1] = 333333;
		(*blk2)[0] = 444444;

		assert((*blk2)[3] == 111111);
		assert((*blk2)[2] == 222222);
		assert((*blk2)[1] == 333333);
		assert((*blk2)[0] == 444444);

		assert((*blk1)[0] == 111111);
		assert((*blk1)[1] == 222222);
		assert((*blk1)[2] == 333333);
		assert((*blk1)[3] == 444444);

		blk1->release();
		blk2->release();

		std::cout << "[V] UniversalMachine::RawBlock ok."
				<< std::endl << std::endl;
	}

	void TestSuite::testLazyBlock() {
		std::cout << "testing lazy block..." << std::endl;

		static const Word testSize = 4;

		LazyBlock *blk1 = new LazyBlock(testSize);

		assert(blk1);

		(*blk1)[0] = 111111;
		(*blk1)[1] = 222222;
		(*blk1)[2] = 333333;
		(*blk1)[3] = 444444;

		assert((*blk1)[0] == 111111);
		assert((*blk1)[1] == 222222);
		assert((*blk1)[2] == 333333);
		assert((*blk1)[3] == 444444);

		LazyBlock *blk2 = new LazyBlock(*blk1);
		LazyBlock *blk3 = new LazyBlock(*blk1);
		LazyBlock *blk4 = new LazyBlock(*blk2);

		assert(blk2);
		assert(blk3);
		assert(blk4);

		assert((*blk2)[0] == 111111);
		assert((*blk2)[1] == 222222);
		assert((*blk2)[2] == 333333);
		assert((*blk2)[3] == 444444);

		assert((*blk3)[0] == 111111);
		assert((*blk3)[1] == 222222);
		assert((*blk3)[2] == 333333);
		assert((*blk3)[3] == 444444);

		assert((*blk4)[0] == 111111);
		assert((*blk4)[1] == 222222);
		assert((*blk4)[2] == 333333);
		assert((*blk4)[3] == 444444);

		(*blk2)[3] = 111111;
		(*blk2)[2] = 222222;
		(*blk2)[1] = 333333;
		(*blk2)[0] = 444444;

		assert((*blk1)[0] == 111111);
		assert((*blk1)[1] == 222222);
		assert((*blk1)[2] == 333333);
		assert((*blk1)[3] == 444444);

		assert((*blk2)[3] == 111111);
		assert((*blk2)[2] == 222222);
		assert((*blk2)[1] == 333333);
		assert((*blk2)[0] == 444444);

		assert((*blk3)[0] == 111111);
		assert((*blk3)[1] == 222222);
		assert((*blk3)[2] == 333333);
		assert((*blk3)[3] == 444444);

		assert((*blk4)[0] == 111111);
		assert((*blk4)[1] == 222222);
		assert((*blk4)[2] == 333333);
		assert((*blk4)[3] == 444444);

		(*blk1)[0] = 555555;
		(*blk1)[1] = 666666;
		(*blk1)[2] = 777777;
		(*blk1)[3] = 888888;

		assert((*blk1)[0] == 555555);
		assert((*blk1)[1] == 666666);
		assert((*blk1)[2] == 777777);
		assert((*blk1)[3] == 888888);

		assert((*blk2)[3] == 111111);
		assert((*blk2)[2] == 222222);
		assert((*blk2)[1] == 333333);
		assert((*blk2)[0] == 444444);

		assert((*blk3)[0] == 111111);
		assert((*blk3)[1] == 222222);
		assert((*blk3)[2] == 333333);
		assert((*blk3)[3] == 444444);

		assert((*blk4)[0] == 111111);
		assert((*blk4)[1] == 222222);
		assert((*blk4)[2] == 333333);
		assert((*blk4)[3] == 444444);

		(*blk4)[0] = 111;
		(*blk4)[1] = 222;
		(*blk4)[2] = 333;
		(*blk4)[3] = 444;

		assert((*blk1)[0] == 555555);
		assert((*blk1)[1] == 666666);
		assert((*blk1)[2] == 777777);
		assert((*blk1)[3] == 888888);

		assert((*blk2)[3] == 111111);
		assert((*blk2)[2] == 222222);
		assert((*blk2)[1] == 333333);
		assert((*blk2)[0] == 444444);

		assert((*blk3)[0] == 111111);
		assert((*blk3)[1] == 222222);
		assert((*blk3)[2] == 333333);
		assert((*blk3)[3] == 444444);

		assert((*blk4)[0] == 111);
		assert((*blk4)[1] == 222);
		assert((*blk4)[2] == 333);
		assert((*blk4)[3] == 444);

		(*blk3)[0] = 1;
		(*blk3)[1] = 2;
		(*blk3)[2] = 3;
		(*blk3)[3] = 4;

		assert((*blk1)[0] == 555555);
		assert((*blk1)[1] == 666666);
		assert((*blk1)[2] == 777777);
		assert((*blk1)[3] == 888888);

		assert((*blk2)[3] == 111111);
		assert((*blk2)[2] == 222222);
		assert((*blk2)[1] == 333333);
		assert((*blk2)[0] == 444444);

		assert((*blk3)[0] == 1);
		assert((*blk3)[1] == 2);
		assert((*blk3)[2] == 3);
		assert((*blk3)[3] == 4);

		assert((*blk4)[0] == 111);
		assert((*blk4)[1] == 222);
		assert((*blk4)[2] == 333);
		assert((*blk4)[3] == 444);

		delete blk1;
		delete blk2;
		delete blk3;
		delete blk4;

		std::cout << "[V] UniversalMachine::LazyBlock ok."
				<< std::endl << std::endl;
	}

	void TestSuite::testMemMan() {
		std::cout << "testing memory manager..." << std::endl;

		std::cout << "\trunning MemMan test, " << std::endl <<
				"\tcross your fingers to prevent segfaults" << std::endl;

		static const Word testSize = 4;

		std::cout << "\t\tulmp. " << std::endl;

		MemMan *mm = new MemMan();

		std::cout << "\t\twrrr... " << std::endl;

		Word i = mm->allocatePage(testSize);
		Word j = mm->allocatePage(testSize);
		Word k = mm->allocatePage(testSize);

		std::cout << "\t\twrrrpppl... " << std::endl;

		assert(i);
		assert(j);
		assert(k);

		std::cout << "\t\twurrm... " << std::endl;

		mm->getWritable(i, 0) = 111111;
		mm->getWritable(i, 1) = 222222;
		mm->getWritable(i, 2) = 333333;
		mm->getWritable(i, 3) = 444444;

		mm->getWritable(j, 0) = 555555;
		mm->getWritable(j, 1) = 666666;
		mm->getWritable(j, 2) = 777777;
		mm->getWritable(j, 3) = 888888;

		mm->getWritable(k, 3) = 111111;
		mm->getWritable(k, 2) = 222222;
		mm->getWritable(k, 1) = 333333;
		mm->getWritable(k, 0) = 444444;

		std::cout << "\t\tgrngl. " << std::endl;

		assert((*mm)(i, 0) == 111111);
		assert((*mm)(i, 1) == 222222);
		assert((*mm)(i, 2) == 333333);
		assert((*mm)(i, 3) == 444444);

		assert((*mm)(j, 0) == 555555);
		assert((*mm)(j, 1) == 666666);
		assert((*mm)(j, 2) == 777777);
		assert((*mm)(j, 3) == 888888);

		assert((*mm)(k, 3) == 111111);
		assert((*mm)(k, 2) == 222222);
		assert((*mm)(k, 1) == 333333);
		assert((*mm)(k, 0) == 444444);

		std::cout << "\t\twrngwrngwrng!" << std::endl;
		
		mm->setRootPage(i);

		std::cout << "\t\turrrmpl." << std::endl;
		
		assert((*mm)(0, 0) == 111111);
		assert((*mm)(0, 1) == 222222);
		assert((*mm)(0, 2) == 333333);
		assert((*mm)(0, 3) == 444444);

		std::cout << "\t\twrrfl!" << std::endl;
		
		mm->getWritable(0, 3) = 111111;
		mm->getWritable(0, 2) = 222222;
		mm->getWritable(0, 1) = 333333;
		mm->getWritable(0, 0) = 444444;

		std::cout << "\t\tbork bork bork" << std::endl;
		
		assert((*mm)(i, 0) == 111111);
		assert((*mm)(i, 1) == 222222);
		assert((*mm)(i, 2) == 333333);
		assert((*mm)(i, 3) == 444444);

		assert((*mm)(0, 3) == 111111);
		assert((*mm)(0, 2) == 222222);
		assert((*mm)(0, 1) == 333333);
		assert((*mm)(0, 0) == 444444);

		mm->setRootPage(j);

		assert((*mm)(0, 0) == 555555);
		assert((*mm)(0, 1) == 666666);
		assert((*mm)(0, 2) == 777777);
		assert((*mm)(0, 3) == 888888);

		mm->setRootPage(k);

		assert((*mm)(0, 3) == 111111);
		assert((*mm)(0, 2) == 222222);
		assert((*mm)(0, 1) == 333333);
		assert((*mm)(0, 0) == 444444);

		std::cout << "\t\tmrrrrgl... " << std::endl;

		mm->deallocatePage(i);
		mm->deallocatePage(j);
		mm->deallocatePage(k);

		std::cout << "\t\twarrrble!.." << std::endl;

		delete mm;

		std::cout << "\teverything's just peachy." << std::endl;

		std::cout << "[V] UniversalMachine::MemMan ok."
				<< std::endl << std::endl;
	}

	void TestSuite::testPageLoader(const std::string &filename) {
		std::cout << "testing page loader..." << std::endl;

		MemMan *mm = new MemMan();

		std::cout << "\ttesting page loader (using \"" << filename
				<< "\")..." << std::endl;

		mm->deallocatePage(PageLoader::loadPage(mm, filename));

		std::cout << "\tpage loaded successfully, good enough for me."
				<< std::endl;

		std::cout << "\ttesting page loader (using \"sandmark.umz\")..."
				<< std::endl;

		Word sandmarkId = PageLoader::loadPage(mm, "sandmark.umz");

		std::cout << "\tpage loaded successfully, testing data..."
				<< std::endl;

		std::cout << "\t[!] to compile for big-endian hardware, "
				<< std::endl <<
				"\t    remove the corresponding #define in um.hpp "
				<< std::endl << "\t    (ugly, I know)" << std::endl;

		std::cout << "\t[!] mixed-endian hardware " <<
				"will need a different" << std::endl <<
				"\t    EndiannessFixer::fixEndianness()" << std::endl;

		std::cout << "\t\t0x0000 (0x080000d0)" << std::endl;

		assert((*mm)(sandmarkId, 0) == 0x080000d0);

		std::cout << "\t\t0x0001 (0x300000c0)" << std::endl;

		assert((*mm)(sandmarkId, 1) == 0x300000c0);

		std::cout << "\t\t0x0002 (0xd2000014)" << std::endl;

		assert((*mm)(sandmarkId, 2) == 0xd2000014);

		std::cout << "\t\t0x3000 (0x708769e9)" << std::endl;

		assert((*mm)(sandmarkId, 0x3000) == 0x708769e9);

		std::cout << "\tdata ok." << std::endl;

		mm->deallocatePage(sandmarkId);

		delete mm;

		std::cout << "[V] UniversalMachine::PageLoader ok."
				<< std::endl << std::endl;
	}

	void TestSuite::testRegisterBlock() {
		std::cout << "testing register block..." << std::endl;

		static const Byte testSize = 4;

		RegisterBlock *blk1 = new RegisterBlock(testSize);

		assert(blk1);

		(*blk1)[0] = 111111;
		(*blk1)[1] = 222222;
		(*blk1)[2] = 333333;
		(*blk1)[3] = 444444;

		assert((*blk1)[0] == 111111);
		assert((*blk1)[1] == 222222);
		assert((*blk1)[2] == 333333);
		assert((*blk1)[3] == 444444);

		(*blk1)[3] = 111111;
		(*blk1)[2] = 222222;
		(*blk1)[1] = 333333;
		(*blk1)[0] = 444444;

		assert((*blk1)[3] == 111111);
		assert((*blk1)[2] == 222222);
		assert((*blk1)[1] == 333333);
		assert((*blk1)[0] == 444444);

		delete blk1;

		std::cout << "[V] UniversalMachine::RegisterBlock ok."
				<< std::endl << std::endl;
	}

	void TestSuite::testConsole() {
		std::cout << "testing console..." << std::endl;

		std::cout << "\ttesting console output..." << std::endl;

		std::cout << "\t\t[!] (I'm expecting ASCII stdout and stdin, "
				<< std::endl << "\t\t    tough luck if yours aren't)"
				<< std::endl;
		
		Console::init();

		std::cout << "\t\tThe next two lines should be identical:"
				<< std::endl;

		std::string test(
				"\t\t\"A quick brown fox jumps over a lazy dog.\"\n");

		std::cout << test;

		for (std::string::iterator i = test.begin(); i != test.end(); ++ i)
			Console::putByte(*i);

		std::cout << "\t\tI'll just assume everything worked fine."
				<< std::endl;
		std::cout << "\t\tIf not, you probably can't read this either."
				<< std::endl;

		std::cout << "\tconsole output works as expected." << std::endl;

		std::cout << "\ttesting console input (sort of)..."
				<< std::endl;

		for (std::string::iterator i = test.begin(); i != test.end(); ++ i) {
			Byte chr;

			std::ungetc(*i, stdin);
			chr = Console::getByte();

			assert(chr == *i);
		}

		std::cout << "\tconsole input works as expected." << std::endl;

		Console::release();

		std::cout << "[V] UniversalMachine::Console ok."
				<< std::endl << std::endl;
	}

R_UM_NS_END

#endif
