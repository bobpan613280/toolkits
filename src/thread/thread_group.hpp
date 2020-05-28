#ifndef __THREAD_GROUP__
#define __THREAD_GROUP__

#include <thread>
#include <mutex>
#include <list>
#include <memory>

namespace std
{
	//兼容boost::thread_group
	//使用std::thread代替boost::thread,std::mutex代替boost::shared_mutex
	class thread_group
	{
	private:
		thread_group(thread_group const&);
		thread_group& operator=(thread_group const&);
	public:
		thread_group() {}
		~thread_group()
		{
			for (auto it = threads.begin(), end = threads.end(); it != end; ++it)
			{
				delete *it;
			}
		}

		template<typename F>
		std::thread* create_thread(F threadfunc)
		{
			std::lock_guard<std::mutex> guard(m);
			std::auto_ptr<std::thread> new_thread(new std::thread(threadfunc));
			threads.push_back(new_thread.get());
			return new_thread.release();
		}

		void add_thread(std::thread* thrd)
		{
			if (thrd)
			{
				std::lock_guard<std::mutex> guard(m);
				threads.push_back(thrd);
			}
		}

		void remove_thread(std::thread* thrd)
		{
			std::lock_guard<std::mutex> guard(m);
			/*auto it = std::find(threads.begin(), threads.end(), thrd);
			if (it != threads.end())
			{
				threads.erase(it);
			}*/
			for (auto it = threads.begin(), end = threads.end(); it != end; ++it)
			{
				if ((*it) == thrd)
				{
					threads.erase(it);
					break;
				}
			}
		}

		void join_all()
		{
			std::lock_guard<std::mutex> guard(m);
			for (auto it = threads.begin(), end = threads.end(); it != end; ++it)
			{
				(*it)->join();
			}
		}

		size_t size() const
		{
			std::lock_guard<std::mutex> guard(m);
			return threads.size();
		}

	private:
		std::list<std::thread*> threads;
		mutable std::mutex m;
	};
}

#endif
