//
// Created by student on 28/04/18.
//

#include <wait.h>
#include <zconf.h>
#include <cstring>
#include "slave.hpp"
#include "parser.hpp"
#include "console.hpp"

std::string Plazza::Console::find_first_not_like(std::string cmd, std::string filter)
{
	std::size_t i = 0;

	while (filter.find(cmd[i]) != std::string::npos)
		++i;
	return (&cmd[i]);
}

bool Plazza::Console::managePrompt(std::string &line)
{
	std::string cmd = line.substr(0, line.find(';'));

	while (!cmd.empty()) {
		std::vector<std::string> file{};
		Plazza::Parser::Information info = Plazza::Parser::Information::NONE;

		this->filFileArray(cmd, file, info);
		this->giveTheseFiles(file, info);
		line = line.substr(cmd.size());
		line = this->find_first_not_like(line, "; ");
		cmd = line.substr(0, line.find(';'));
	}
	return true;
}

bool Plazza::Console::prompt()
{
	int status = 0;
	std::string buffer;

	while (std::getline(std::cin, buffer)) {
		if (!this->managePrompt(buffer))
			return false;
	}
	wait(&status);
	return true;
}
