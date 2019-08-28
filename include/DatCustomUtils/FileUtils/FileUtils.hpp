#ifndef __DAT_CUSTOM_FILE_UTILS_HPP__
#define __DAT_CUSTOM_FILE_UTILS_HPP__
#include <stddef.h>
namespace DatCustom{
	namespace FileUtils{
		/**
		 * @brief read the whole file at once
		 * @param pSize the pointer to the data in which contains the size of data
		 * note: data is malloc-ed
		 */
		void* readWholeFileAtOnce(const char* filename, size_t* pSize);
	}
}
#endif
