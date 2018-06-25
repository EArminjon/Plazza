//
// Created by student on 02/05/18.
//

#ifndef CPP_PLAZZA_THREADPOOL_HPP
#define CPP_PLAZZA_THREADPOOL_HPP

#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "parser.hpp"

namespace Plazza {
	class threadPool {
	private:
		typedef struct s_threadData {
			std::string file;
			void (Plazza::Parser::*function)(const std::string &line);
		} threadData;
		std::vector<Plazza::threadPool::threadData> taskQueue;
		std::vector<std::pair<std::thread::id, bool>> threadState;
		//std::vector<std::thread> pool;
		std::size_t _nbThread;
		std::mutex doorQueue;
		std::mutex getDoor;
		size_t _capacity;
	public:
		explicit threadPool(std::size_t nbThread);
		~threadPool();
		void threadFunction();
		void createPool(std::size_t nbThread);
		void addTask(const std::string &file, Plazza::Parser::ptrFunction);
		void setReady(const std::thread::id &id, const bool &state);
		std::size_t getInfo();
		std::size_t getReady();
		std::size_t getNotReady();
		Plazza::threadPool::threadData getTask();
		bool stopThread;
	};
}

#endif //CPP_PLAZZA_THREADPOOL_HPP
