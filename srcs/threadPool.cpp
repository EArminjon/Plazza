//
// Created by student on 02/05/18.
//

#include <zconf.h>
#include <iostream>
#include "threadPool.hpp"

std::size_t Plazza::threadPool::getInfo()
{
	/* return how many more taks, this pool can accept */
	if (this->stopThread)
		return (0);
	return this->_capacity - this->taskQueue.size();
}

void Plazza::threadPool::setReady(const std::thread::id &id, const bool &state)
{
	for (auto &row : this->threadState) {
		if (row.first == id)
			row.second = state;
	}
}

std::size_t Plazza::threadPool::getReady()
{
	std::size_t nbThreadReady = 0;

	for (auto &row : this->threadState) {
		if (row.second)
			++nbThreadReady;
	}
	return nbThreadReady;
}

std::size_t Plazza::threadPool::getNotReady()
{
	std::size_t nbThreadReady = 0;

	for (auto &row : this->threadState) {
		if (!row.second)
			++nbThreadReady;
	}
	return nbThreadReady;
}

void Plazza::threadPool::addTask(const std::string &file, Plazza::Parser::ptrFunction function)
{
	std::mutex door;
	Plazza::threadPool::threadData task;

	task.file = file;
	task.function = function;
	door.lock();
	this->taskQueue.push_back(task);
	door.unlock();
}


Plazza::threadPool::threadData Plazza::threadPool::getTask()
{
	Plazza::threadPool::threadData task;

	task.function = nullptr;
	task.file = std::string();
	this->getDoor.lock();
	if (this->taskQueue.empty()) {
		this->getDoor.unlock();
		return task;
	}
	task = *this->taskQueue.begin();
	this->taskQueue.erase(this->taskQueue.begin());
	this->getDoor.unlock();
	return task;
}

void Plazza::threadPool::threadFunction()
{
	Plazza::Parser parser;

	this->setReady(std::this_thread::get_id(), true);
	while (!this->stopThread) {
		Plazza::threadPool::threadData task;
		task.function = nullptr;
		task.file = std::string();

		while (task.function == nullptr) {
			task = this->getTask();
			if (task.function == nullptr && this->stopThread)
				return ;
			usleep(1000);
		}
		this->setReady(std::this_thread::get_id(), false);
		std::cerr << "Thread:" << std::this_thread::get_id() << " take:" << task.file << std::endl;
		std::string file = task.file;
		Plazza::Parser::ptrFunction function = task.function;
		(parser.*function)(file); /* launch parser */
		this->setReady(std::this_thread::get_id(), true);
	}
}

void Plazza::threadPool::createPool(std::size_t nbThread)
{
	for (std::size_t index = 0 ; index < nbThread ; ++index) {
		std::thread thread(&Plazza::threadPool::threadFunction, this);
			thread.detach();
			this->threadState.emplace_back(thread.get_id(), true);
	}
}

Plazza::threadPool::threadPool(std::size_t nbThread)
{
	this->stopThread = false;
	this->_nbThread = nbThread;
	this->_capacity = (2 * nbThread);
	this->createPool(nbThread);
}

Plazza::threadPool::~threadPool() = default;