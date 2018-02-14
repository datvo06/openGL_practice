#ifndef __DAT_STRING_UTILS
#define __DAT_STRING_UTILS
#include <string>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
// For C
#include <stdio.h>
#include <stdlib.h>


namespace DatTools{
	namespace Util{
		std::string delete_white_space(const std::string& the_str);
		std::vector<std::string> split_whitespace(std::string the_str);
		std::vector<std::string> split_endline(std::string the_str);
		/**
		 * @brief  Trim whitespaces from a string (both left and right)
		 */
		std::string trim(const std::string& str);
		std::string string_from_file_cpp(std::string filename="input.txt");
		char* string_from_file_c(const char *filename);
		void string_to_file(std::string input_string, std::string filename="input.txt");
	/**
	 * @brief  Does the string contain a whitespace?
	 */
	bool contains_whitespace(const std::string&);
	}
}

#endif
