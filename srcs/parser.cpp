//
// Created by student on 02/05/18.
//

#include <regex>
#include <fstream>
#include <iostream>
#include <log.hpp>
#include "parser.hpp"

Plazza::Parser::Parser()
{
	this->info["PHONE_NUMBER"] = Plazza::Parser::Information::PHONE_NUMBER;
	this->info["EMAIL_ADDRESS"] = Plazza::Parser::Information::EMAIL_ADDRESS;
	this->info["IP_ADDRESS"] = Plazza::Parser::Information::IP_ADDRESS;
}

Plazza::Parser::~Parser()
{}

void Plazza::Parser::phoneParse(const std::string &file_name)
{
	Plazza::log log("output.txt");
	std::ifstream file(file_name);
	std::regex pattern(R"((\d{10})|((\d{2} ){4}\d{2}))");
	std::smatch matches;

	if (file.is_open()) {
		std::string line;
		while (getline (file,line)) {
			if (std::regex_search(line, matches, pattern)) {
				std::string tmp = matches[0];
				std::cout << tmp << std::endl;
				log.writeLog("File:\"" + file_name + R"(", type:"PHONE_NUMBER", data:")" + tmp + "\";");
			}
		}
		file.close();
	}
	//log.writeLog("File:\"" + file_name + R"(", type:"PHONE_NUMBER", data:"";)");
}

void Plazza::Parser::ipParse(const std::string &file_name)
{
	Plazza::log log("output.txt");
	std::ifstream file(file_name);
	std::regex pattern(R"((\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}))");
	std::smatch matches;

	if (file.is_open()) {
		std::string line;
		while (getline (file,line)) {
			if (std::regex_search(line, matches, pattern)) {
				std::string tmp = matches[0];
				std::cout << tmp << std::endl;
				log.writeLog("File:\"" + file_name + R"(", type:"IP_ADDRESS", data:")" + tmp + "\";");
			}
		}
		file.close();
	}
	//log.writeLog("File:\"" + file_name + R"(", type:"IP_ADDRESS", data:"";)");
}

void Plazza::Parser::emailParse(const std::string &file_name)
{
	Plazza::log log("output.txt");
	std::ifstream file(file_name);
	std::regex pattern("[a-zA-Z0-9_.-]+@[a-zA-Z0-9_.-]+");
	std::smatch matches;

	if (file.is_open()) {
		std::string line;
		while (getline (file,line)) {
			if (std::regex_search(line, matches, pattern)) {
				std::string tmp = matches[0];
				std::cout << tmp << std::endl;
				log.writeLog("File:\"" + file_name + R"(", type:"EMAIL_ADDRESS", data:")" + tmp + "\";");
			}
		}
		file.close();
	}
	//log.writeLog("File:\"" + file_name + R"(", type:"EMAIL_ADDRESS", data:"";)");
}
