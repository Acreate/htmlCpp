#ifndef HTMLWORKTHREADPOOL_H_H_HEAD__FILE__
#define HTMLWORKTHREADPOOL_H_H_HEAD__FILE__
#pragma once

#include <functional>
#include <mutex>
#include <random>
#include "HtmlWorkThread.h"

#if WIN32
		#include "windows.h"
#elif __linux__
		#include "unistd.h"
#endif

namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT HtmlWorkThreadPool {
	public:
#if WIN32
		/// <summary>
		/// 获取 cpu 核心数量
		/// </summary>
		/// <returns>核心数量</returns>
		inline static size_t getSystemCupCount( ) {
			SYSTEM_INFO sysInfo;
			GetSystemInfo( &sysInfo );
			return sysInfo.dwNumberOfProcessors;
		}
#elif __linux__
		/// <summary>
		/// 获取 cpu 核心数量
		/// </summary>
		/// <returns>核心数量</returns>
		inline static size_t getSystemCupCount( ) {
			return sysconf( _SC_NPROCESSORS_CONF);
		}
#endif
	public:
		/// <summary>
		///	轮询回调函数
		/// 1。当前工作线程
		/// 2。剩余工作数
		///	3。正在工作数
		/// </summary>
		using TThreadCall = std::function< void( cylHtmlTools::HtmlWorkThreadPool *html_work_thread_pool, const unsigned long long &workCount, const unsigned long long &currentWorkCount ) >;
	private:
		/// <summary>
		/// 工作对象
		/// </summary>
		std::vector< HtmlWorkThread::TThreadCall > works;
		// 当前工作线程
		std::vector< HtmlWorkThread * > workThreads;
		// 闲置调用，与参数 function_call 同效(function_call 不存在时，使用该调用)
		std::shared_ptr< TThreadCall > idleTimeCall;
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
		std::chrono::milliseconds callSepMilliseconds = std::chrono::milliseconds( 1000 ); // 调用间隔-毫秒
		std::chrono::seconds idleTimeCallWaiteSec = std::chrono::seconds( 1 ); // 等待时校验秒
	public:
		HtmlWorkThreadPool( );
		virtual ~HtmlWorkThreadPool( );
		std::chrono::seconds getIdleTimeCallWaiteSec( ) const { return idleTimeCallWaiteSec; }
		void setIdleTimeCallWaiteSec( const std::chrono::seconds &waite_sec ) { if( waite_sec.count( ) > 0 ) idleTimeCallWaiteSec = waite_sec; }
		void setIdleTimeCallWaiteSec( const long long &waite_sec ) { if( waite_sec > 0 ) idleTimeCallWaiteSec = std::chrono::seconds( waite_sec ); }
		size_t getThreadCount( ) const { return works.size( ); };
		size_t getWorkCount( ) const { return workCount; }
		void setWorkCount( const size_t work_count ) { workCount = work_count; }
		bool isRun( ) const;
		bool isFinish( ) const;
		bool isInit( ) const;
		bool isNone( ) const;
		void setIdleTimeCall( const TThreadCall &idle_time_call ) {
			if( !idleTimeCall )
				idleTimeCall = std::make_shared< TThreadCall >( );
			*idleTimeCall = idle_time_call;
		}
		std::chrono::milliseconds getCallSepMilliseconds( ) const { return callSepMilliseconds; }
		void setCallSepMilliseconds( const std::chrono::milliseconds &call_sep_milliseconds ) { callSepMilliseconds = call_sep_milliseconds; }
		void setCallSepMilliseconds( const size_t &call_sep_milliseconds ) { callSepMilliseconds = std::chrono::milliseconds( call_sep_milliseconds ); }
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
				start( HtmlWorkThreadPool::getSystemCupCount( ), function_call );
			else
				start( workCount, function_call );

		}
		/// <summary>
		/// 开始任务，每次轮询都会调用一次 function_call
		/// </summary>
		/// <param name="work_count">任务数量</param>
		inline void start( const size_t work_count ) {
			if( workCount == 0 )
				start( HtmlWorkThreadPool::getSystemCupCount( ), nullptr );
			else
				start( workCount, nullptr );

		}
		/// <summary>
		/// 开始任务，每次轮询都会调用一次 function_call
		/// </summary>
		inline void start( ) {
			if( workCount == 0 )
				start( HtmlWorkThreadPool::getSystemCupCount( ), nullptr );
			else
				start( workCount, nullptr );

		}

		/// <summary>
		/// 设置任务数量，并且开始任务，每次轮询都会调用一次 function_call
		/// </summary>
		/// <param name="work_count">任务数量</param>
		/// <param name="function_call">轮训回调函数</param>
		inline void start( const size_t work_count, const cylHtmlTools::HtmlWorkThreadPool::TThreadCall &function_call );
		/// <summary>
		/// 等待任务总结-阻塞型
		/// </summary>
		void waiteOverJob( );
		/// <summary>
		/// 是否完成任务-非阻塞型
		/// </summary>
		/// <returns>正在任务完成时返回 true，未进行任务，或者任务未结束返回 false</returns>
		bool isOverJob( );
	};
}

#endif // HTMLWORKTHREADPOOL_H_H_HEAD__FILE__
