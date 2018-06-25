//
// Created by student on 04/05/18.
//

#include <sstream>
#include <cstring>
#include <zconf.h>
#include "serializer.hpp"

Plazza::serializer::serializer()
{}

Plazza::serializer::~serializer()
{}

Plazza::serializer &operator>>(const std::string &str, Plazza::serializer &response) /* unserialize */
{
	std::string sub;

	sub = str.substr(str.find("type:") + strlen("type:"));
	sub = sub.substr(0, sub.find(','));
	response.type = static_cast<Plazza::Console::tansfertType>(std::stoi(sub));
	sub.clear();
	sub = str.substr(str.find("info:") + strlen("info:"));
	sub = sub.substr(0, sub.find(','));
	response.info = static_cast<Plazza::Parser::Information>(std::stoi(sub));

	sub.clear();
	sub = str.substr(str.find("files:[") + strlen("files:["));
	sub = sub.substr(0, sub.find(']'));

	std::string tmp = sub;
	bool mbreak = false;
	while (!tmp.empty() && !mbreak) {
		sub.clear();
		tmp = tmp.substr(tmp.find(",\"") + 2);
		if (tmp.find(",\"") == std::string::npos)
			mbreak = true;
		sub = tmp.substr(0, tmp.find('\"'));
		response.files.push_back(sub);
	}
	return response;
}

std::string &operator<<(std::string &str, const Plazza::serializer &response) /* serialize */
{
	std::stringstream ss;
	std::stringstream msg;

	ss << "[";
	for (auto &row : response.files) {
		ss << ",";
		ss << "\"" << row << "\"";
	}
	ss << "]";
	msg << "{type:" << response.type << ",info:" <<  response.info << ",files:" << ss.str() << "}" << std::endl;
	str = msg.str();
	return str;
}
