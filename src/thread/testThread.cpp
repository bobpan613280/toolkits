/*!
 * \file testThread.cpp
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#include "testThread.h"

testThread::testThread()
{
	m_pThreadPool = std::make_shared<ThreadPool>(3);
	m_pThreadPool->SetMaxSize(5);
	m_pThreadPool->init();

	std::thread thrd(&testThread::_Thread,this);
	bCancel = false;
	thrd.detach();
}

testThread::~testThread()
{
	m_pThreadPool->stop();
	m_pThreadPool->wait();
}

void testThread::InitandRun()
{
	bCancel = false;
	m_pThreadPool->wait();
}

void testThread::_Thread()
{
	static int num = 0;
	while (!bCancel)
	{
		//std::cout << "test times:"<<num++ << std::endl;
// 		if (num % 2 == 0)
// 		{
// 	 		Task task = std::bind(&testThread::ProcFun1, this, num);
// 	 		m_pThreadPool->AddNewTask(task);
// 		}
// 		if (num % 3 == 0)
// 		{
// 			Task task = std::bind(&testThread::ProcFun2, this, num,num+1);
// 			m_pThreadPool->AddNewTask(task);
// 		}
		if (num % 4 == 0)
		{
			CallBackfuncT calFunc = std::bind(&testThread::CallBackFun, this, std::placeholders::_1);
			Task task = std::bind(&testThread::ProcFun3, this, num, num + 1, calFunc);
			m_pThreadPool->AddNewTask(task);
		}
		num++;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void testThread::setCallbackFunc(const std::function<void(void)> &func)
{
	//sig.connect(func);
}

void testThread::ProcFun1(int m_num)
{
	std::cout << "ProcFun1 run 3" << std::endl;
}

void testThread::ProcFun2(int m_num, int num2)
{
	std::cout << "ProcFun2 run 88888888" << std::endl;
}


void testThread::ProcFun3(int m_num, int num3, CallBackfuncT callBack)
{
	std::cout << "ProcFunc3 -> " << m_num << std::endl;
	if (callBack)
		callBack(num3);
}

void testThread::CallBackFun(int num)
{
	std::cout << "CallBackFun run " << num << std::endl;
}
