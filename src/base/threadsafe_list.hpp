/*!
 * \file threadsafe_list.hpp
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#ifndef _THREADSAFE_LIST_H_
#define _THREADSAFE_LIST_H_

#include <exception>
#include <memory>
#include <list>
#include <mutex>
#include <iostream>
namespace SystemLog {

	struct empty_list :std::exception
	{
		const char* what()  const throw() { return "empty list"; };
	};

	template<typename T>
	class threadsafe_list
	{
	private:
		std::list<T> dataList;
		mutable std::mutex m;
	public:
		threadsafe_list() {}
		threadsafe_list(const threadsafe_list& other)
		{
			std::lock_guard<std::mutex> guard(m);
			dataList = other.dataList;
		}
		threadsafe_list& operator=(const threadsafe_list&) = delete;
		bool push_back(T new_val)
		{
			std::lock_guard<std::mutex> guard(m);
			dataList.push_back(new_val);
			return true;
		}
		std::shared_ptr<T> pop_front()
		{
			std::lock_guard<std::mutex> guard(m);
			if (dataList.empty())
				throw empty_list();
			std::shared_ptr<T> const res(std::make_shared<T>(dataList.front()));
			dataList.pop_front();
			return res;
		}
		bool pop_front(T& value)
		{
			std::lock_guard<std::mutex> guard(m);
			if (dataList.empty())
			{
				//throw empty_list();
				return false;
			}
			value = dataList.front();
			dataList.pop_front();
			return true;
		}
		bool empty() const
		{
			std::lock_guard<std::mutex> guard(m);
			return dataList.empty();
		}
		int getSize() const
		{
			std::lock_guard<std::mutex> guard(m);
			return dataList.size();
		}
		void clear() {
			std::lock_guard<std::mutex> guard(m);
			dataList.clear();
		}
	};
}
#endif

