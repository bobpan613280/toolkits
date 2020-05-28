/*!
 * \file testThread.h
 * \date 2018/11/30
 *
 * \author pan yingdong
 * Contact: bob.pan@hzleaper.com
 *
 *
 * \note 
*/
#ifndef _TESTTHREAD_H_
#define _TESTTHREAD_H_

#include <iostream>
#include "ThreadPool.hpp"
#include <functional>
typedef std::function<void(int)> CallBackfuncT;
class testThread
{
public:
	testThread();
	~testThread();
	void InitandRun();
	void _Thread();
	void setCallbackFunc(const std::function<void(void)> &func);
	

	void ProcFun1(int m_num);
	void ProcFun2(int m_num,int num2);
	void ProcFun3(int m_num,int num3, CallBackfuncT callBack);
	void CallBackFun(int num);
private:
	bool bCancel{ false };
	std::shared_ptr<ThreadPool> m_pThreadPool;
};

#endif