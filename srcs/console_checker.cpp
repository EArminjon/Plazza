//
// Created by student on 28/04/18.
//

#include <ostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <arpa/inet.h>
#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include "console.hpp"

bool Plazza::Console::isFileExist(const std::string &file_name)
{
	struct stat buffer{};
	return (stat(file_name.c_str(), &buffer) == 0);
}

bool Plazza::Console::filFileArray(std::string cmd, std::vector<std::string> &file,
								   Plazza::Parser::Information &data)
{
	Plazza::Parser parser;
	std::stringstream ss;
	std::string file_name;

	ss << cmd.c_str();
	ss >> file_name;
	while (!file_name.empty()) {
		file.push_back(file_name);
		file_name.clear();
		ss >> file_name;
	}
	if (file.size() < 2) {
		std::cerr << "Bad formatting command line." << std::endl;
		return false;
	}
	std::string info = file[file.size() - 1];

	if (parser.info.find(info) == parser.info.end()) {
		std::cerr << "Error: Last argument cannot be used to parse files:" << info << "." << std::endl;
		return false;
	}
	data = parser.info[info];
	file.erase(file.end());
	file.erase(std::remove_if(file.begin(), file.end(), [](const auto& row) {
				return !Plazza::Console::isFileExist(row);
			}), file.end());
	if (!file.empty())
		return true;
	std::cerr << "Not enough valid files." << std::endl;
	return false;
}