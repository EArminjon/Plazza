//
// Created by student on 24/04/18.
//

#ifndef CPP_CONSOLE_HPP
#define CPP_CONSOLE_HPP

#include <ostream>
#include <iostream>
#include <vector>
#include <string>
#include "parser.hpp"

namespace Plazza {
	class Console {
	public:
		/* ATTRIBUTES */
		const std::string _replyFile = "fifo/reply";
		const std::string _requestFile = "fifo/request";
		enum tansfertType {
			COMMAND,
			TRANSFER,
			NONE
		};
		enum descripteur {
			FD_READ = 0,
			FD_WRITE = 1,
		};
		typedef struct s_comPipe {
			int read;
			int write;
			int childPid;
		} comPipe;
		/* CONSTRUCT && DESTRUCT */
		explicit Console(std::size_t nb);
		~Console();
		/* CHECKER */
		static bool isFileExist(const std::string &file_name);
		/* FUNCTIONS */
		bool prompt();
		bool managePrompt(std::string &buffer);
		std::string find_first_not_like(std::string cmd, std::string filter);
		void giveTheseFiles(std::vector<std::string> &file, enum Plazza::Parser::Information &info);
		/*bool forkProcess(std::vector<std::string> &file, enum Plazza::Parser::Information &info);
		void manageChild(std::vector<std::string> &file, enum Plazza::Parser::Information &info);*/
		void deleteFifoFiles();
		bool filFileArray(std::string cmd, std::vector<std::string> &file, enum Plazza::Parser::Information &info);
		void createChild();
		void launchChild(const std::string &write, const std::string &read);
		void dispatchFiles(std::vector<std::string> &file, enum Plazza::Parser::Information &info, std::size_t *slots);
		void giveFiles(const Plazza::Console::comPipe &child, std::size_t &slot, std::vector<std::string> &file, const Plazza::Parser::Information &info);
		void giveOneFile(const Plazza::Console::comPipe &child, std::size_t &slot, std::vector<std::string> &file, const Plazza::Parser::Information &info);
		std::size_t getChildInfo(long pos);
		static bool isChildAlive(Plazza::Console::comPipe child);
		void removeDeadChild();
	private:
		std::size_t nbChilds;
		std::vector<Plazza::Console::comPipe> childs;
		std::size_t threadNb;
	};
}

#endif //CPP_CONSOLE_HPP
