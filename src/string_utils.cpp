#include <Dat/string_utils.h>


std::string DatTools::Util::delete_white_space(const std::string& the_str){
	std::string the_new_str = the_str;
	boost::algorithm::replace_all(the_new_str, "\n", "");
	boost::algorithm::replace_all(the_new_str, "\t", "");
	boost::algorithm::replace_all(the_new_str, " ", "");
	return the_new_str;
}


std::vector<std::string> DatTools::Util::split_whitespace(std::string the_str){
	std::vector<std::string> list_split;
	boost::algorithm::split(list_split, the_str, [](char c){return c == ' '|| c== '\n' || c=='\t';});
	std::vector<std::string>list_split_final;
	for(auto it:list_split){
		if (it.size()>0) list_split_final.push_back(it);
	}
	return list_split_final;
}


std::vector<std::string> DatTools::Util::split_endline(std::string the_str){
	std::vector<std::string> list_split;
	boost::algorithm::split(list_split, the_str, [](char c){return c== '\n';});
	std::vector<std::string>list_split_final;
	for(auto it:list_split){
		if (it.size()>0) list_split_final.push_back(it);
	}
	return list_split_final;
}


std::string DatTools::Util::trim(const std::string& str)
{
	std::string result = str;

	// trim from start
	result.erase(result.begin(), std::find_if(result.begin(), result.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));

	// trim from end
	result.erase(std::find_if(result.rbegin(), result.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), result.end());

	return result;
}


std::string DatTools::Util::string_from_file_cpp(std::string filename){
	std::ifstream ifs;
	ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::string theStr;
	try{
		ifs.open(filename, std::ios_base::in| std::ios_base::binary);

		std::stringstream buffer;
		buffer << ifs.rdbuf();
		theStr = buffer.str();
		ifs.close();
	}
	catch(std::ifstream::failure e){
		std::cout << "ERROR::STRING_FROM_FILE::NOT_SUCCESSFULLY_READ" << std::endl;
	}
	return theStr;
}


char* DatTools::Util::string_from_file_c(const char *filename){
	char *buffer = NULL;
	int string_size, read_size;
	FILE* handler = fopen(filename, "r");
	if (handler){
		// Seek the last byte of the file
		fseek(handler, 0, SEEK_END);
		string_size = ftell(handler);
		// go back to the start of the file
		rewind(handler);
		// Allocate a string that can hold it all
		buffer = (char*) malloc(sizeof(char) * (string_size + 1));
		// read it all in one operation
		read_size = fread(buffer, sizeof(char), string_size, handler);
		// fread doesn't set it so put a \0 in the last position
		buffer[string_size] = '\0';
		if (string_size != read_size)
		{
			// Some thing went wrong, thow away the memory and set buffer to null
			free(buffer);
			buffer = NULL;
		}
		// Always close the handler
		fclose(handler);
	}
	return buffer;
}



void DatTools::Util::string_to_file(std::string input_string, std::string filename){
	std::fstream ofs;
	ofs.open(filename, std::ios_base::out);
	ofs << input_string;
	ofs.close();
}


bool DatTools::Util::contains_whitespace(const std::string& str)
{
	return str.end() != std::find_if(str.begin(), str.end(), std::ptr_fun<int, int>(std::isspace));
}
