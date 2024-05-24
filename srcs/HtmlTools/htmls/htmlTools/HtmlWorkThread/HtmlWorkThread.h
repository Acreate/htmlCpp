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
		using T_Start_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType *data ) >;
		template< class TThreadDataType >
		using T_Current_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType *data, const time_t *startTime ) >;
		template< class TThreadDataType >
		using T_Finish_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType *data ) >;
		using Start_Thread_Run = T_Start_Thread_Run< TDataType >;
		using Current_Thread_Run = T_Current_Thread_Run< TDataType >;
		using Finish_Thread_Run = T_Finish_Thread_Run< TDataType >;
		enum Html_Work_Status {
			None // 没有任何状态-无法使用
			, Init // 正在初始化
			, Finish // 已经完成任务
			, Run // 正在运行
		};
	private:
		Start_Thread_Run startThreadRun; // 线程开始之后调用
		Current_Thread_Run currentThreadRun; // 正式工作
		Finish_Thread_Run finishThreadRun; // 线程完成之后调用
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
		HtmlWorkThread( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, TDataType *data );
		/// <summary>
		/// 初始化一个对象
		/// </summary>
		HtmlWorkThread( );
		virtual ~HtmlWorkThread( );
	public:// - 属性
		/// <summary>
		/// 重新初始化线程
		/// </summary>
		/// <param name="start_thread_run">线程开始回调函数</param>
		/// <param name="finish_thread_run">线程结束回调函数</param>
		/// <param name="current_thread_run">线程正式工作的回调函数</param>
		/// <param name="data">传递的数据指针</param>
		/// <returns>成功返回 true</returns>
		bool reInit( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, TDataType *data );
		/// <summary>
		/// 获取开始回调
		/// </summary>
		/// <returns>开始回调</returns>
		Start_Thread_Run getStartThreadRun( ) const { return startThreadRun; }
		/// <summary>
		/// 设置开始回调
		/// </summary>
		/// <param name="start_thread_run">开始回调</param>
		void setStartThreadRun( const Start_Thread_Run &start_thread_run ) { startThreadRun = start_thread_run; }
		/// <summary>
		/// 获取运行回调
		/// </summary>
		/// <returns>运行回调</returns>
		Current_Thread_Run getCurrentThreadRun( ) const { return currentThreadRun; }
		/// <summary>
		/// 设置运行回调
		/// </summary>
		/// <param name="current_thread_run">运行回调</param>
		void setCurrentThreadRun( const Current_Thread_Run &current_thread_run ) { currentThreadRun = current_thread_run; }
		/// <summary>
		/// 获取完成回调
		/// </summary>
		/// <returns>完成回调</returns>
		Finish_Thread_Run getFinishThreadRun( ) const { return finishThreadRun; }
		/// <summary>
		/// 设置完成回调
		/// </summary>
		/// <param name="finish_thread_run">完成回调</param>
		void setFinishThreadRun( const Finish_Thread_Run &finish_thread_run ) { finishThreadRun = finish_thread_run; }
		/// <summary>
		/// 获取数值指针
		/// </summary>
		/// <returns>数据指针</returns>
		TDataType * getData( ) const { return data; }
		/// <summary>
		/// 设置数据指针
		/// </summary>
		/// <param name="data">数据指针</param>
		void setData( TDataType *const data ) { this->data = data; }
	public:	// 状态
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
	public: // 工作
		/// <summary>
		/// 线程开始工作
		/// </summary>
		void start( );

		/// <summary>
		/// 等待线程结束
		/// </summary>
		/// <returns>线程运行时间</returns>
		time_t wait( );
	};

	template< class TDataType >
	class HTMLTOOLS_EXPORT HtmlWorkThread< std::shared_ptr< TDataType > > {
	public:
		template< class TThreadDataType >
		using T_Start_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType &data ) >;
		template< class TThreadDataType >
		using T_Current_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType &data, const time_t *startTime ) >;
		template< class TThreadDataType >
		using T_Finish_Thread_Run = std::function< void( const HtmlWorkThread *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, TThreadDataType &data ) >;
		using Start_Thread_Run = T_Start_Thread_Run< std::shared_ptr< TDataType > >;
		using Current_Thread_Run = T_Current_Thread_Run< std::shared_ptr< TDataType > >;
		using Finish_Thread_Run = T_Finish_Thread_Run< std::shared_ptr< TDataType > >;
		enum Html_Work_Status {
			None // 没有任何状态-无法使用
			, Init // 正在初始化
			, Finish // 已经完成任务
			, Run // 正在运行
		};
	private:
		Start_Thread_Run startThreadRun; // 线程开始之后调用
		Current_Thread_Run currentThreadRun; // 正式工作
		Finish_Thread_Run finishThreadRun; // 线程完成之后调用
		std::thread *thread; // 线程对象
		Html_Work_Status workStatus; // 记录线程状态
		time_t *startTime; // 记录线程开始的运行时间
		time_t *endTime; // 记录线程结束的运行时间
		std::mutex *mutexHtmlWorkThread; // 对象线程锁
		std::mutex *mutexStdThread; // 线程锁
		std::shared_ptr< TDataType > data; // 数据指针
	public:
		/// <summary>
		/// 初始化一个对象
		/// </summary>
		/// <param name="start_thread_run">线程开始回调函数</param>
		/// <param name="finish_thread_run">线程结束回调函数</param>
		/// <param name="current_thread_run">线程正式工作的回调函数</param>
		/// <param name="data">传递的数据指针</param>
		HtmlWorkThread( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, std::shared_ptr< TDataType > &data );
		/// <summary>
		/// 初始化一个对象
		/// </summary>
		HtmlWorkThread( );
		virtual ~HtmlWorkThread( );
	public: // 属性
		/// <summary>
		/// 重新初始化线程
		/// </summary>
		/// <param name="start_thread_run">线程开始回调函数</param>
		/// <param name="finish_thread_run">线程结束回调函数</param>
		/// <param name="current_thread_run">线程正式工作的回调函数</param>
		/// <param name="data">传递的数据指针</param>
		/// <returns>成功返回 true</returns>
		bool reInit( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, std::shared_ptr< TDataType > &data );
		/// <summary>
		/// 获取开始回调
		/// </summary>
		/// <returns>开始回调</returns>
		Start_Thread_Run getStartThreadRun( ) const { return startThreadRun; }
		/// <summary>
		/// 设置开始回调
		/// </summary>
		/// <param name="start_thread_run">开始回调</param>
		void setStartThreadRun( const Start_Thread_Run &start_thread_run ) { startThreadRun = start_thread_run; }
		/// <summary>
		/// 获取运行回调
		/// </summary>
		/// <returns>运行回调</returns>
		Current_Thread_Run getCurrentThreadRun( ) const { return currentThreadRun; }
		/// <summary>
		/// 设置运行回调
		/// </summary>
		/// <param name="current_thread_run">运行回调</param>
		void setCurrentThreadRun( const Current_Thread_Run &current_thread_run ) { currentThreadRun = current_thread_run; }
		/// <summary>
		/// 获取完成回调
		/// </summary>
		/// <returns>完成回调</returns>
		Finish_Thread_Run getFinishThreadRun( ) const { return finishThreadRun; }
		/// <summary>
		/// 设置完成回调
		/// </summary>
		/// <param name="finish_thread_run">完成回调</param>
		void setFinishThreadRun( const Finish_Thread_Run &finish_thread_run ) { finishThreadRun = finish_thread_run; }
		/// <summary>
		/// 获取数值指针
		/// </summary>
		/// <returns>数据指针</returns>
		std::shared_ptr< TDataType > getData( ) const { return data; }
		/// <summary>
		/// 设置数据指针
		/// </summary>
		/// <param name="data">数据指针</param>
		void setData( const std::shared_ptr< TDataType > &data ) { this->data = data; }
	public: // - 状态
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
	public: // - 工作
		/// <summary>
		/// 线程开始工作
		/// </summary>
		void start( );
		/// <summary>
		/// 等待线程结束
		/// </summary>
		/// <returns>线程运行时间</returns>
		time_t wait( );
	};
}

#endif // HTMLWORKTHREAD_H_H_HEAD__FILE__
