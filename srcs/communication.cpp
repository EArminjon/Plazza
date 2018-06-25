//
// Created by student on 03/05/18.
//

#include <zconf.h>
#include <iostream>
#include <fcntl.h>
#include <mutex>
#include <csignal>
#include "communication.hpp"

Plazza::communication::communication()
{
	this->mySignal();
}

Plazza::communication::~communication() = default;

int Plazza::communication::openCom(const std::string &file, const int &flag)
{
	std::mutex door;

	door.lock();
	int fd = open(file.c_str(), flag);
	door.unlock();
	if (fd == -1) {
		std::cerr << "Communication: open fail." << std::endl;
		return -1;
	}
	return fd;
}

int Plazza::communication::closeCom(int &fd)
{
	std::mutex door;

	door.lock();
	if (fcntl(fd, F_GETFD) != -1)
		return close(fd);
	door.unlock();
	return 0;
}

void Plazza::communication::myHandler(int s)
{
	(void) s;
}

int Plazza::communication::mySignal()
{
	struct sigaction sigIntHandler{};

	sigIntHandler.sa_handler = &Plazza::communication::myHandler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGALRM, &sigIntHandler, nullptr);
	return 0;
}

void Plazza::communication::writeMsg(const int &fd, const std::string &msg)
{
	std::mutex readDoor;

	readDoor.lock();
	write(fd, msg.c_str(), msg.size());
	readDoor.unlock();
}

std::string Plazza::communication::readMsg(const int &fd)
{
	char *buffer = nullptr;
	std::size_t size = 0;
	ssize_t res = 0;
	std::mutex door;

	door.lock();
	FILE *f = fdopen(dup(fd), "r");
	door.unlock();
	if (f == nullptr) {
		std::cerr << "Communication: fdopen failed" << std::endl;
		return std::string();
	}
	alarm(5);
	res = getline(&buffer, &size, f);
	alarm(0);
	fclose(f);
	if (res <= 0 || buffer == nullptr) {
		if (buffer != nullptr)
			free(buffer);
		return std::string();
	}
	std::string result(buffer);
	free(buffer);
	return result;
}
