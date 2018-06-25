//
// Created by student on 03/05/18.
//

#include <wait.h>
#include <zconf.h>
#include <cstring>
#include <serializer.hpp>
#include <algorithm>
#include "slave.hpp"
#include "parser.hpp"
#include "console.hpp"

void Plazza::Console::giveFiles(const Plazza::Console::comPipe &child,
								std::size_t &slot,
								std::vector<std::string> &file,
								const Plazza::Parser::Information &info)
{
	Plazza::serializer object;

//	std::cout << "On va envoyer des fichiers: fileNumber:" << file.size() << " slot:" << slot << std::endl;
	for (; slot > 0 && !file.empty(); --slot) {
		//std::cout << "DELETE:" << file.begin()->data() << std::endl;
		object.files.emplace_back(file.begin()->data());
		file.erase(file.begin());
	}
	object.info = info;
	object.type = Plazza::Console::TRANSFER;
	Plazza::communication com;
	std::string msg;
	msg << object;
	com.writeMsg(child.write, msg);
	com.readMsg(child.read);
}

void Plazza::Console::giveOneFile(const Plazza::Console::comPipe &child,
								  std::size_t &slot,
								  std::vector<std::string> &file,
								  const Plazza::Parser::Information &info)
{
	Plazza::serializer object;

	slot -= 1;
	object.info = info;
	object.type = Plazza::Console::TRANSFER;
	object.files.emplace_back(file.begin()->data());
	file.erase(file.begin());
	Plazza::communication com;
	std::string msg;

	msg << object;
	com.writeMsg(child.write, msg);
	com.readMsg(child.read);
}


void Plazza::Console::dispatchFiles(std::vector<std::string> &file,
									enum Plazza::Parser::Information &info,
									std::size_t *slots)
{
	std::size_t minChildSlot = 0;
	std::size_t maxFile = 0;

	if (this->childs.size() == 1 && slots[0] > 0)
		return this->giveFiles(this->childs[0], slots[0], file, info);
	for (std::size_t pos = 0; pos < this->childs.size(); ++pos)
		maxFile += slots[pos];
	for (; maxFile > 0 && !file.empty(); --maxFile) {
		for (std::size_t pos = 0; pos < this->childs.size(); ++pos) {
			if (slots[pos] > slots[minChildSlot] && slots[pos] > 0)
				minChildSlot = pos;
		}
		if (slots[minChildSlot] == 0)
			return;
		giveOneFile(this->childs[minChildSlot], slots[minChildSlot], file,
					info);
		slots[minChildSlot] -= 1;
	}
}

bool Plazza::Console::isChildAlive(Plazza::Console::comPipe child)
{
	int status;
	pid_t result = waitpid(child.childPid, &status, WNOHANG);

	if (result > 0) {
		close(child.read);
		close(child.write);
		return false;
	}
	return true;
}

void Plazza::Console::removeDeadChild()
{
	this->childs.erase(std::remove_if(this->childs.begin(), this->childs.end(), [](const auto& row) {
		return !Plazza::Console::isChildAlive(row);
	}), this->childs.end());
}

void Plazza::Console::giveTheseFiles(std::vector<std::string> &file,
									 enum Plazza::Parser::Information &info)
{
	while (!file.empty()) {
		this->removeDeadChild();
		size_t slots[this->childs.size()];
		for (auto child = this->childs.begin();
			 !this->childs.empty() && !file.empty() &&
			 child != this->childs.end(); ++child) {
			slots[child - childs.begin()] = this->getChildInfo(
					child - childs.begin());
		}
		this->dispatchFiles(file, info, slots);
		if (!file.empty())
			this->createChild();
	}
}