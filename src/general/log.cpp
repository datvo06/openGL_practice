#include <stdio.h>
#include <CustomizedUtils/logutils.h>


void DatCustom_debug(const char* message, ...){
	va_list args;
  va_start(args, message);
	printf(message, args);
	va_end(args);
}
