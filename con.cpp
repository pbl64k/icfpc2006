#include "con.hpp"

R_UM_NS_START

#ifdef R_UM_OPT_LINUX_NOLINEBUF_STDIN
	termios Console::termiosOld_;
	FILE *Console::snapshot_;
#endif

	void Console::init() {
#ifdef R_UM_OPT_STDOUT_SNAPSHOT
		startSnapshot();
#endif

#ifdef R_UM_OPT_LINUX_NOLINEBUF_STDIN
		reconfigTerm();
#endif
	}

	void Console::release() {
#ifdef R_UM_OPT_LINUX_NOLINEBUF_STDIN
		restoreTerm();
#endif

#ifdef R_UM_OPT_STDOUT_SNAPSHOT
		stopSnapshot();
#endif
	}

	void Console::putByte(Word character) {
		Byte byteChar = static_cast<Byte>(character);

		assert(byteChar == character);

#ifdef R_UM_OPT_STDOUT_SNAPSHOT
		addToSnapshot(byteChar);
#endif

		assert(std::putchar(byteChar) == byteChar);
		assert(! std::fflush(stdout));
	}

	Word Console::getByte() {
		char character;
		Word input;

		Word charsRead = std::fread(&character, sizeof(character), 1, stdin);

		if (charsRead == 1) {
			input = static_cast<Word>(character);
		}
		else
		{
			if (std::feof(stdin)) {
				input = static_cast<Word>(0) - 1;
			}
			else {
				assert(false);
			}
		}

#ifdef R_UM_OPT_STDOUT_SNAPSHOT
		stopSnapshot();
		startSnapshot();
#endif

		return input;
	}

	void Console::reconfigTerm() {
#ifdef R_UM_OPT_LINUX_NOLINEBUF_STDIN
		termios termiosUm;

		assert(! tcgetattr(STDIN_FILENO, &termiosOld_));

		termiosUm = termiosOld_;
		termiosUm.c_lflag &= ~ ICANON;
		termiosUm.c_cc[VMIN] = 1;
		termiosUm.c_cc[VTIME] = 0;

		assert(! tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosUm));
#endif
	}

	void Console::restoreTerm() {
#ifdef R_UM_OPT_LINUX_NOLINEBUF_STDIN
		assert(! tcsetattr(STDIN_FILENO, TCSAFLUSH, &termiosOld_));
#endif
	}

	void Console::startSnapshot() {
#ifdef R_UM_OPT_STDOUT_SNAPSHOT
		snapshot_ = std::fopen("snapshot.um", "w");

		assert(isSnapshotValid());
#endif
	}

	void Console::stopSnapshot() {
#ifdef R_UM_OPT_STDOUT_SNAPSHOT
		assert(isSnapshotValid());

		assert(! std::fclose(snapshot_));
#endif
	}

	void Console::addToSnapshot(Byte character) {
#ifdef R_UM_OPT_STDOUT_SNAPSHOT
		assert(isSnapshotValid());

		assert(std::fputc(character, snapshot_) == character);
		assert(! std::fflush(snapshot_));
#else
		assert(character && false);
#endif
	}

	bool Console::isSnapshotValid() {
#ifdef R_UM_OPT_STDOUT_SNAPSHOT
		return snapshot_ != NULL;
#else
		return false;
#endif
	}

R_UM_NS_END
