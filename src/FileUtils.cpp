#include <DatCustomUtils/FileUtils/FileUtils.hpp>
#include <stdio.h>
#include <stdlib.h>

void* DatCustom::FileUtils::readWholeFileAtOnce(const char* filename, size_t* pSize){
		FILE *f = fopen(filename, "rb");
		fseek(f, 0, SEEK_END);
		size_t fSize;
		if (pSize == 0){
			pSize = &fSize;
		}
		*pSize = ftell(f);
		fseek(f, 0, SEEK_SET);  //same as rewind(f);
		void* data = malloc(*pSize);
		fread(data, *pSize, 1, f);
		fclose(f);
		return data;
}
