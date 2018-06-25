//
// Created by student on 02/05/18.
//

#include <sstream>
#include <fstream>
#include <zconf.h>
#include <cstring>
#include <fcntl.h>
#include <serializer.hpp>
#include "slave.hpp"

Plazza::Slave::Slave(std::size_t nbThread, const std::string &write, const std::string &read)
{
	this->_pipes.write = Plazza::communication::openCom(write, O_WRONLY);
	this->_pipes.read = Plazza::communication::openCom(read, O_RDONLY);
	this->poolManager = std::move(std::make_unique<threadPool>(nbThread));
	this->threadFunction = {{Plazza::Parser::Information::PHONE_NUMBER,  &Plazza::Parser::phoneParse},
							{Plazza::Parser::Information::EMAIL_ADDRESS, &Plazza::Parser::emailParse},
							{Plazza::Parser::Information::IP_ADDRESS,    &Plazza::Parser::ipParse}};
}

Plazza::Slave::~Slave()
{
	Plazza::communication::closeCom(this->_pipes.write);
	Plazza::communication::closeCom(this->_pipes.read);
}

void Plazza::Slave::writeToMasterGetInfo(Plazza::communication &com)
{
	std::stringstream ss;
	std::size_t result = this->getInfo();
	ss << result << "\n";
	com.writeMsg(this->_pipes.write, ss.str());
}

void Plazza::Slave::writeToThreadPoolTask(Plazza::communication &com, const std::string &response)
{
	com.writeMsg(this->_pipes.write, "OK\n");
	Plazza::serializer object;

	response >> object;
	for (auto &file_name : object.files) {
		std::string file = file_name;
		Plazza::Parser::ptrFunction function = this->threadFunction[object.info];
		this->poolManager->addTask(file, function);
	}
}

void Plazza::Slave::waitTask()
{
	std::mutex communicationDoor;
	Plazza::communication com;
	Plazza::serializer object;
	std::string response;

	while (true) { /* keep son alive */
		communicationDoor.lock();
		response = com.readMsg(this->_pipes.read);
		if (response.empty() || response[0] == '\n') {
			communicationDoor.unlock();
			break; /* kill son */
		}
		response >> object;
		if (object.type == Plazza::Console::COMMAND)
			this->writeToMasterGetInfo(com);
		else if (object.type == Plazza::Console::TRANSFER)
			this->writeToThreadPoolTask(com, response);
		communicationDoor.unlock();
	}
	this->poolManager->stopThread = true;
	while (this->poolManager->getNotReady() > 0)
		usleep(10000);
}

std::size_t Plazza::Slave::getInfo()
{
	return this->poolManager->getInfo();
}

void Plazza::Slave::addFiles(std::vector<std::string> &files,
							 Plazza::Parser::Information &info)
{
	for (auto &file : files)
		this->poolManager->addTask(file, this->threadFunction[info]);
}

void Plazza::Slave::addFile(const std::string &file,
							Plazza::Parser::Information &info)
{
	this->poolManager->addTask(file, this->threadFunction[info]);
}

