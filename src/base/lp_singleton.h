#ifndef _LP_SIGLETON_SYS_H_
#define _LP_SIGLETON_SYS_H_

#include <mutex>
#include <atomic>
namespace SystemLog {
	template<class T>
	class lp_singleton_sys
	{
	public:
		static T* instance()
		{
			T *sin = sys_this.load(std::memory_order_acquire);

			if (!sin) {
				std::lock_guard<std::mutex> locker(sys_mutex);
				sin = sys_this.load(std::memory_order_relaxed);
				if (!sin) {
					sin = new T;
					sys_this.store(sin, std::memory_order_release);
				}
			}

			return sin;
		}

		static void uninstance()
		{
			T *sin = sys_this.load(std::memory_order_relaxed);
			if (sin) {
				std::lock_guard<std::mutex> locker(sys_mutex);
				delete sin;
				sin = nullptr;
			}
		}

	protected:
		lp_singleton_sys() = default;
		virtual ~lp_singleton_sys() = default;
	private:
		lp_singleton_sys(const T&) = delete;
		T& operator=(const T&) = delete;

		static std::atomic<T*> sys_this;
		static std::mutex sys_mutex;
	};

	template<class T>
	std::atomic<T*> lp_singleton_sys<T>::sys_this;

	template<class T>
	std::mutex lp_singleton_sys<T>::sys_mutex;
}
#endif
