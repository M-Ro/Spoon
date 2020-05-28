#include "StreamParser.h"

StreamParser::StreamParser(const std::string &filepath)
{
	a_filepath = filepath;
	a_file = nullptr;
}