/*
** EPITECH PROJECT, 2018
** main
** File description:
** main
*/

#include <iostream>
#include "console.hpp"

int main(int ac, char **av)
{
	if (ac != 2) {
		std::cerr << "Program need one number argument." << std::endl;
		return 84;
	}
	int thread = std::stoi(av[1]);
	if (thread <= 0) {
		std::cerr << "Argument must be positif" << std::endl;
		return 84;
	}
	Plazza::Console console(static_cast<std::size_t>(thread));
	console.prompt();
	return 0;
}
