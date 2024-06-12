#ifndef HTMLWORKTHREADPOOL_H_H_HEAD__FILE__
#define HTMLWORKTHREADPOOL_H_H_HEAD__FILE__
#pragma once

#include <functional>
#include <mutex>
#include <random>
#include "HtmlWorkThread.h"

namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT HtmlWorkThreadPool {
	public:
		/// <summary>
		///	轮询回调函数
		/// 1。当前工作线程
		/// 2。剩余工作数
		///	3。正在工作数
		/// </summary>
		using TThreadCall = std::function< void( cylHtmlTools::HtmlWorkThreadPool *html_work_thread_pool, const unsigned long long &workCount, const unsigned long long &crrentWorkCount ) >;
	private:
		/// <summary>
		/// 工作对象
		/// </summary>
		std::vector< HtmlWorkThread::TThreadCall > works;
		// 当前工作线程
		std::vector< HtmlWorkThread * > workThreads;
		/// <summary>
		/// 工作锁
		/// </summary>
		std::mutex *mutexHtmlWorkThread;
		/// <summary>
		/// 状态
		/// </summary>
		HtmlWorkThread::Html_Work_Status workStatus;
		/// <summary>
		/// 任务派发线程
		/// </summary>
		cylHtmlTools::HtmlWorkThread taskDistributeThread;
		/// <summary>
		/// 用户调用线程
		/// </summary>
		cylHtmlTools::HtmlWorkThread userCallThread;
		// 工作数量
		size_t workCount;
		size_t currentWorkThread;  // 当前工作线程数量
	public:
		HtmlWorkThreadPool( );
		virtual ~HtmlWorkThreadPool( );
		size_t getWorkCount( ) const { return workCount; }
		void setWorkCount( const size_t work_count ) { workCount = work_count; }
		bool isRun( ) const;
		bool isFinish( ) const;
		bool isInit( ) const;
		bool isNone( ) const;
		/// <summary>
		/// 追加一个任务
		/// </summary>
		/// <param name="work">追加的任务</param>
		bool appendWork( const HtmlWorkThread::TThreadCall &work );
		/// <summary>
		/// 开始任务，每次轮询都会调用一次 function_call
		/// </summary>
		/// <param name="function_call">轮询调用函数</param>
		inline void start( const cylHtmlTools::HtmlWorkThreadPool::TThreadCall &function_call ) {
			if( workCount == 0 )
				start( 8, function_call, std::chrono::milliseconds( 200 ) );
			else
				start( workCount, function_call, std::chrono::milliseconds( 200 ) );

		}
		/// <summary>
		/// 设置任务数量，并且开始任务，每次轮询都会调用一次 function_call
		/// </summary>
		/// <param name="work_count">任务数量</param>
		/// <param name="function_call">轮训回调函数</param>
		inline void start( const size_t work_count, const cylHtmlTools::HtmlWorkThreadPool::TThreadCall &function_call ) {
			start( work_count, function_call, std::chrono::milliseconds( 200 ) );
		}
		/// <summary>
		/// 设置任务数量，并且开始任务，每次轮询都会调用一次 function_call
		/// </summary>
		/// <param name="work_count">任务数量</param>
		/// <param name="function_call">轮训回调函数</param>
		/// <param name="mis">睡眠时间，毫秒</param>
		void start( const size_t work_count, const cylHtmlTools::HtmlWorkThreadPool::TThreadCall &function_call, const std::chrono::milliseconds &mis );
		/// <summary>
		/// 等待任务总结
		/// </summary>
		void waiteOverJob( );
	};
}

#endif // HTMLWORKTHREADPOOL_H_H_HEAD__FILE__
