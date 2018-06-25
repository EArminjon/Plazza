//
// Created by student on 03/05/18.
//

#ifndef CPP_PLAZZA_LOG_HPP
#define CPP_PLAZZA_LOG_HPP

#include <iostream>
#include <fstream>

namespace Plazza {
	class log {
	public:
		explicit log(const std::string &file);
		~log();
		void writeLog(const std::string &msg);
	private:
		std::string logFile;
	};
}

#endif //CPP_PLAZZA_LOG_HPP
