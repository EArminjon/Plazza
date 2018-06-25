//
// Created by student on 04/05/18.
//

#ifndef CPP_PLAZZA_SERIALIZER_HPP
#define CPP_PLAZZA_SERIALIZER_HPP

#include <string>
#include <vector>
#include "parser.hpp"
#include "console.hpp"

namespace Plazza {
	class serializer {
	public:
		serializer();
		~serializer();
		std::vector<std::string> files;
		enum Plazza::Parser::Information info;
		enum Plazza::Console::tansfertType type;
	};
}
Plazza::serializer &operator>>(const std::string &str, Plazza::serializer &response);
std::string &operator<<(std::string &str, const Plazza::serializer &response);

#endif //CPP_PLAZZA_SERIALIZER_HPP
