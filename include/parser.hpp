//
// Created by student on 02/05/18.
//

#ifndef CPP_PLAZZA_PARSER_HPP
#define CPP_PLAZZA_PARSER_HPP

#include <map>
#include <string>

namespace Plazza {
	class Parser {
	public:
		Parser();
		~Parser();
		void ipParse(const std::string &file_name);
		void phoneParse(const std::string &file_name);
		void emailParse(const std::string &file_name);
		using ptrFunction = void (Plazza::Parser::*)(const std::string &line);
		enum Information {
			PHONE_NUMBER,
			EMAIL_ADDRESS,
			IP_ADDRESS,
			NONE
		};
		std::map<std::string, Plazza::Parser::Information> info;
	};
}

#endif //CPP_PLAZZA_PARSER_HPP
