//
// Created by student on 03/05/18.
//

#ifndef CPP_PLAZZA_COMMUNICATION_HPP
#define CPP_PLAZZA_COMMUNICATION_HPP

#include <string>

namespace Plazza {
	class communication {
	public:
		communication();
		~communication();
		std::string readMsg(const int &fd);
		void writeMsg(const int &fd, const std::string &msg);
		static void myHandler(int s);
		int mySignal();
		static int openCom(const std::string &file, const int &flag);
		static int closeCom(int &fd);
	};
}

#endif //CPP_PLAZZA_COMMUNICATION_HPP
