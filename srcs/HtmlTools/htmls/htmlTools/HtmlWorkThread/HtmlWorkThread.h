#ifndef HTMLWORKTHREAD_H_H_HEAD__FILE__
#define HTMLWORKTHREAD_H_H_HEAD__FILE__
#pragma once
#include <functional>
#include <thread>

#include "../../../nameSpace/cylHtmlTools.h"
namespace std {
	class mutex;
}
namespace std {
	class thread;
}
namespace cylHtmlTools {

	template< class TDataType >
	class HTMLTOOLS_EXPORT HtmlWorkThread {

	};
	template< class TDataType >
	class HTMLTOOLS_EXPORT HtmlWorkThread< TDataType * > {
	public:
		template< class TThreadDataType >
		using Start_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType data ) >;
		template< class TThreadDataType >
		using Current_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType data, const time_t *startTime ) >;
		template< class TThreadDataType >
		using Finish_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType data ) >;

		enum Html_Work_Status {
			None // 没有任何状态-无法使用
			, Init // 正在初始化
			, Finish // 已经完成任务
			, Run // 正在运行
		};
	private:
		Start_Thread_Run< TDataType * > startThreadRun; // 线程开始之后调用
		Current_Thread_Run< TDataType * > currentThreadRun; // 正式工作
		Finish_Thread_Run< TDataType * > finishThreadRun; // 线程完成之后调用
		std::thread *thread; // 线程对象
		Html_Work_Status workStatus; // 记录线程状态
		time_t *startTime; // 记录线程开始的运行时间
		time_t *endTime; // 记录线程结束的运行时间
		std::mutex *mutexHtmlWorkThread; // 对象线程锁
		std::mutex *mutexStdThread; // 线程锁
		TDataType *data; // 数据指针
	public:
		/// <summary>
		/// 初始化一个对象
		/// </summary>
		/// <param name="start_thread_run">线程开始回调函数</param>
		/// <param name="finish_thread_run">线程结束回调函数</param>
		/// <param name="current_thread_run">线程正式工作的回调函数</param>
		/// <param name="data">传递的数据指针</param>
		HtmlWorkThread( const Start_Thread_Run< TDataType * > &start_thread_run, const Current_Thread_Run< TDataType * > &current_thread_run, const Finish_Thread_Run< TDataType * > &finish_thread_run, TDataType *data );
		virtual ~HtmlWorkThread( );
		/// <summary>
		/// 重新初始化线程
		/// </summary>
		/// <param name="start_thread_run">线程开始回调函数</param>
		/// <param name="finish_thread_run">线程结束回调函数</param>
		/// <param name="current_thread_run">线程正式工作的回调函数</param>
		/// <param name="data">传递的数据指针</param>
		/// <returns>成功返回 true</returns>
		bool reInit( const Start_Thread_Run< TDataType * > &start_thread_run, const Current_Thread_Run< TDataType * > &current_thread_run, const Finish_Thread_Run< TDataType * > &finish_thread_run, TDataType *data );
		/// <summary>
		/// 线程开始工作
		/// </summary>
		void start( );
		/// <summary>
		/// 是否正在运行
		/// </summary>
		/// <returns>运行返回 true</returns>
		bool isRun( ) const;
		/// <summary>
		/// 是否运行结束
		/// </summary>
		/// <returns>结束返回 true</returns>
		bool isFinish( ) const;
		/// <summary>
		/// 等待线程结束
		/// </summary>
		/// <returns>线程运行时间</returns>
		time_t wait( );
	};
}

#endif // HTMLWORKTHREAD_H_H_HEAD__FILE__
