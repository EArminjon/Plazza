//
// Created by student on 24/04/18.
//

#include <memory>
#include <regex>
#include <arpa/inet.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <communication.hpp>
#include "console.hpp"

void Plazza::Console::deleteFifoFiles()
{
	system("rm -f fifo/*");
}

Plazza::Console::Console(std::size_t nb)
{
	this->deleteFifoFiles();
	this->nbChilds = 0;
	this->threadNb = nb;
}

Plazza::Console::~Console()
{
	for (auto &child : this->childs) {
		Plazza::communication::closeCom(child.write);
		Plazza::communication::closeCom(child.read);
	}
}
