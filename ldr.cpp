#include "ldr.hpp"

R_UM_NS_START

	Word PageLoader::loadPage(MemMan *mm, const std::string &filename) {
		struct stat fileStat;

		assert(! stat(filename.c_str(), &fileStat));

		size_t fileSize = fileStat.st_size;
		Word fileSizeInWords = (fileSize / sizeof(Word)) +
				((fileSize % sizeof(Word)) ? 1 : 0);

		FILE *pageFile = std::fopen(filename.c_str(), "r");

		assert(pageFile != NULL);

		Word newPageId = mm->allocatePage(fileSizeInWords);

		assert(std::fread(&(mm->getWritable(newPageId, 0)), sizeof(char),
				fileSize, pageFile) == fileSize);
		assert(! fclose(pageFile));

		EndiannessFixer::fixEndianness(&(mm->getWritable(newPageId, 0)),
				fileSizeInWords);

		return newPageId;
	}

R_UM_NS_END
