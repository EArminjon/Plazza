//
// Created by student on 03/05/18.
//

#include <sstream>
#include <wait.h>
#include <zconf.h>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <serializer.hpp>
#include "slave.hpp"
#include "parser.hpp"
#include "console.hpp"

void Plazza::Console::launchChild(const std::string &write, const std::string &read)
{
	Plazza::Slave slave(this->threadNb, write, read);

	std::cerr << "Creation fils:" << getpid() << std::endl;
	slave.waitTask();
	exit(0);
}

void Plazza::Console::createChild()
{
	std::stringstream ss_reply;
	std::stringstream ss_request;

	++nbChilds;
	ss_request << this->_requestFile << this->nbChilds << ".txt";
	ss_reply << this->_replyFile << this->nbChilds << ".txt";

	mkfifo(ss_request.str().c_str(), 0666);
	mkfifo(ss_reply.str().c_str(), 0666);
	int pid = fork();
	if (pid == -1) {
		std::cerr << "Fork failed." << std::endl;
		return ;
	} else if (pid == 0) {
		return launchChild(ss_reply.str(), ss_request.str());
	} else {
		Plazza::Console::comPipe pipes;
		pipes.read = Plazza::communication::openCom(ss_reply.str(), O_RDONLY);
		pipes.write = Plazza::communication::openCom(ss_request.str(), O_WRONLY);
		pipes.childPid = pid;
		this->childs.push_back(pipes);
	}
}

std::size_t Plazza::Console::getChildInfo(long pos)
{
	Plazza::serializer object;
	Plazza::communication com;
	Plazza::Console::comPipe child = this->childs[pos];

	object.type = Plazza::Console::COMMAND;
	object.info = Plazza::Parser::NONE;
	object.files = std::vector<std::string>();
	std::string msg;

	msg << object;
	if (!Plazza::Console::isChildAlive(child))
		return (0);
	com.writeMsg(child.write, msg);
	std::string response = com.readMsg(child.read);
	auto slot = static_cast<size_t>(std::stoi(response));
	return (slot);
}