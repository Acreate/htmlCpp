#ifndef HTMLWORKTHREAD_H_H_HEAD__FILE__
#define HTMLWORKTHREAD_H_H_HEAD__FILE__
#pragma once
#include "../../../nameSpace/cylHtmlTools.h"
#include <functional>
#include <thread>

#include "../../../auto_generate_files/export/HtmlTools_export.h" // 导出导入声明
#include <mutex>

namespace cylHtmlTools {

	class HTMLTOOLS_EXPORT HtmlWorkThread {
	public:
		enum Html_Work_Status {
			None // 没有任何状态-无法使用
			, Init // 正在初始化
			, Finish // 已经完成任务
			, Run // 正在运行
		};
		using TThreadCall = std::function< void( HtmlWorkThread * ) >;
	private:
		TThreadCall startThreadRun; // 线程开始之后调用
		TThreadCall currentThreadRun; // 正式工作
		TThreadCall finishThreadRun; // 线程完成之后调用
		std::thread *thread; // 线程对象
		Html_Work_Status workStatus; // 记录线程状态
		std::shared_ptr< std::mutex > mutexHtmlWorkThread; // 对象线程锁
	public:
		/// <summary>
		/// 初始化一个对象
		/// </summary>
		/// <param name="start_thread_run">线程开始回调函数</param>
		/// <param name="finish_thread_run">线程结束回调函数</param>
		/// <param name="current_thread_run">线程正式工作的回调函数</param>
		HtmlWorkThread( const TThreadCall &start_thread_run, const TThreadCall &current_thread_run, const TThreadCall &finish_thread_run );
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
		/// <returns>成功返回 true</returns>
		bool reInit( const cylHtmlTools::HtmlWorkThread::TThreadCall &start_thread_run, const cylHtmlTools::HtmlWorkThread::TThreadCall &current_thread_run, const cylHtmlTools::HtmlWorkThread::TThreadCall &finish_thread_run );
		/// <summary>
		/// 获取开始回调
		/// </summary>
		/// <returns>开始回调</returns>
		cylHtmlTools::HtmlWorkThread::TThreadCall getStartThreadRun( ) const;
		/// <summary>
		/// 设置开始回调
		/// </summary>
		/// <param name="start_thread_run">开始回调</param>
		void setStartThreadRun( const cylHtmlTools::HtmlWorkThread::TThreadCall &start_thread_run );
		/// <summary>
		/// 获取运行回调
		/// </summary>
		/// <returns>运行回调</returns>
		cylHtmlTools::HtmlWorkThread::TThreadCall getCurrentThreadRun( ) const;
		/// <summary>
		/// 设置运行回调
		/// </summary>
		/// <param name="current_thread_run">运行回调</param>
		void setCurrentThreadRun( const cylHtmlTools::HtmlWorkThread::TThreadCall &current_thread_run );
		/// <summary>
		/// 获取完成回调
		/// </summary>
		/// <returns>完成回调</returns>
		TThreadCall getFinishThreadRun( ) const;
		/// <summary>
		/// 设置完成回调
		/// </summary>
		/// <param name="finish_thread_run">完成回调</param>
		void setFinishThreadRun( const TThreadCall &finish_thread_run );
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
		/// <summary>
		/// 是否运行结束
		/// </summary>
		/// <returns>结束返回 true</returns>
		bool isInit( ) const;
		/// <summary>
		/// 是否运行结束
		/// </summary>
		/// <returns>结束返回 true</returns>
		bool isNone( ) const;
	public: // - 工作
		/// <summary>
		/// 线程开始工作
		/// </summary>
		void start( );
		/// <summary>
		/// 等待线程结束
		/// </summary>
		/// <returns>线程运行时间</returns>
		void wait( ) const;

	};
}

#endif // HTMLWORKTHREAD_H_H_HEAD__FILE__
