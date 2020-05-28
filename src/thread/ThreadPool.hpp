/*!
 * \file ThreadPool.hpp
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#ifndef _STREAMTHREAD_H_
#define _STREAMTHREAD_H_
#include <iostream>
//#include <queue>
#include <list>
#include <functional>
#include <condition_variable>

#include "threadsafe_list.hpp"
#include "thread_group.hpp"

typedef std::function<void(void)> Task;

template<typename T>
class TaskList
{
private:
	std::list<T> m_taskList;
	std::mutex m_mutex;
	std::condition_variable_any m_cond;
	int m_maxSize{ 10 };
public:
	void ProcFun() {};
	void setMaxSize(int size = 10) {
		m_maxSize = size;
	}
	bool push_back(const T& task) {
		if (getSize() >= m_maxSize)
			return false;
		std::unique_lock<std::mutex> lock(m_mutex);
		m_taskList.push_back(task);
		m_cond.notify_one();
		return true;
	}
	bool pop_front(T& value) {
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_taskList.size() <= 0)
		{
			return false;
		}
		value = m_taskList.front();
		m_taskList.pop_front();
		return true;
	}
	T pop_front() {
		std::unique_lock<std::mutex> lock(m_mutex);
		if(m_taskList.size()<=0)
		{
			m_cond.wait(lock);
		}
		try {
			T task = m_taskList.front();
			m_taskList.pop_front();
			return task;
		}
		catch (std::exception &e) {
			std::cout << "pop task error" << std::endl;
		}
		T task = std::bind(&TaskList::ProcFun, this);
		return task;
	}
	int getSize()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_taskList.size();
	}
};

class ThreadPool
{
private:
	TaskList<Task> m_taskList;
	std::thread_group m_threadGroup;
	int m_threadNum;
	volatile bool is_run;
	void run() {
		while (is_run) {
			if (m_taskList.getSize() > 0)
			{
				Task task;
				if(m_taskList.pop_front(task))
					task();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			else
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
		}
		std::cout << "threadpool run finish" << std::endl;
	}
public:
	ThreadPool(int num) :m_threadNum(num), is_run(false)//
	{
	}
	~ThreadPool() {
		stop();
	}
	void init()
	{
		if (m_threadNum <= 0) 
			return;
		is_run = true;
		for (int i = 0; i < m_threadNum; i++)
		{
			m_threadGroup.add_thread(new std::thread(&ThreadPool::run, this));
		}
	}

	void stop()
	{
		is_run = false;
	}

	bool AddNewTask(const Task& task) {
		m_taskList.push_back(task);
		return true;
	}
	void SetMaxSize(int size) {
		m_taskList.setMaxSize(size);
	}
	void wait()
	{
		m_threadGroup.join_all();
	}
};
#endif
