//
// Created by student on 03/05/18.
//

#include <fstream>
#include <mutex>
#include <zconf.h>
#include "log.hpp"

Plazza::log::log(const std::string &file)
{
	this->logFile = file;
}
 
Plazza::log::~log()
{
}

void Plazza::log::writeLog(const std::string &msg)
{
	std::ofstream file;
	std::mutex fileDoor;

	fileDoor.lock();
	file.open(this->logFile, std::ios::app);
	if (!file.is_open()) {
		std::cerr << "Open failed." << std::endl;
		fileDoor.unlock();
		return;
	}
	file << msg << std::endl;
	file.close();
	fileDoor.unlock();
}
