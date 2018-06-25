//
// Created by student on 02/05/18.
//

#ifndef CPP_PLAZZA_SLAVE_HPP
#define CPP_PLAZZA_SLAVE_HPP

#include <memory>
#include <map>
#include "communication.hpp"
#include "threadPool.hpp"
#include "console.hpp"

namespace Plazza {
	class Slave {
	public:
		Slave(std::size_t nbThread, const std::string &write, const std::string &read);
		~Slave();
		std::size_t getInfo();
		void addFiles(std::vector<std::string> &files, Plazza::Parser::Information &info);
		void addFile(const std::string &files, Plazza::Parser::Information &info);
		void waitTask();
		void writeToMasterGetInfo(Plazza::communication &com);
		void writeToThreadPoolTask(Plazza::communication &com, const std::string &response);
	private:
		using ptrThreadFunction = void (Plazza::Parser::*)(const std::string &file_name);
		std::map<Plazza::Parser::Information, ptrThreadFunction> threadFunction;
		std::unique_ptr<Plazza::threadPool> poolManager;
		Plazza::Console::comPipe _pipes;
	};
}

#endif //CPP_PLAZZA_SLAVE_HPP
