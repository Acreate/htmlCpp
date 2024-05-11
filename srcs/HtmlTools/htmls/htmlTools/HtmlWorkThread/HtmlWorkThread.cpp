#include "HtmlWorkThread.h"
#include <mutex>
#include <thread>
cylHtmlTools::HtmlWorkThread::HtmlWorkThread( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, void *data ): startThreadRun( start_thread_run )
, finishThreadRun( finish_thread_run )
, data( data )
, currentThreadRun( current_thread_run ) {
	this->workStatus = Init;
	mutexHtmlWorkThread = new std::mutex( );
	mutexStdThread = new std::mutex( );
	this->endTime = nullptr;
	this->startTime = nullptr;
}
cylHtmlTools::HtmlWorkThread::~HtmlWorkThread( ) {
	delete mutexHtmlWorkThread;
	delete mutexStdThread;
	if( thread ) {
		thread->join( );
		delete thread;
	}
}
bool cylHtmlTools::HtmlWorkThread::reInit(
	const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run
	, void *data ) {
	if( isRun( ) || this->workStatus == None )
		return false;
	this->mutexHtmlWorkThread->lock( );
	this->startThreadRun = start_thread_run;
	this->currentThreadRun = current_thread_run;
	this->finishThreadRun = finish_thread_run;
	this->data = data;
	this->mutexHtmlWorkThread->unlock( );
	return true;
}
void cylHtmlTools::HtmlWorkThread::start( ) {
	this->mutexHtmlWorkThread->lock( );
	this->workStatus = Run;
	this->thread = new std::thread( [this]( ) {
		this->startTime = new time_t( std::time( nullptr ) );
		this->startThreadRun( this, this->thread, this->mutexHtmlWorkThread, this->mutexStdThread, this->data );
		this->currentThreadRun( this, this->thread, this->mutexHtmlWorkThread, this->mutexStdThread, this->data, this->startTime );
		this->finishThreadRun( this, this->thread, this->mutexHtmlWorkThread, this->mutexStdThread, this->data );
		this->endTime = new time_t( std::time( nullptr ) );
		this->workStatus = Finish;
	} );
	this->mutexHtmlWorkThread->unlock( );
}
bool cylHtmlTools::HtmlWorkThread::isRun( ) const {
	this->mutexHtmlWorkThread->lock( );
	bool cond = this->workStatus == Run;
	this->mutexHtmlWorkThread->unlock( );
	return cond;
}
bool cylHtmlTools::HtmlWorkThread::isFinish( ) const {
	this->mutexHtmlWorkThread->lock( );
	bool cond = this->workStatus == Finish;
	this->mutexHtmlWorkThread->unlock( );
	return cond;
}
time_t cylHtmlTools::HtmlWorkThread::wait( ) {
	if( isFinish( ) ) {
		this->mutexHtmlWorkThread->lock( );
		auto result = *this->endTime - *this->startTime;
		this->mutexHtmlWorkThread->unlock( );
		return result;
	}
	this->mutexHtmlWorkThread->lock( );
	thread->join( );
	auto result = *this->endTime - *this->startTime;
	this->mutexHtmlWorkThread->unlock( );
	return result;
}
