#include "HtmlWorkThread.h"


cylHtmlTools::HtmlWorkThread::HtmlWorkThread( const std::function< void( ) > &start_thread_run, const std::function< void( ) > &current_thread_run, const std::function< void( ) > &finish_thread_run ) {
	this->workStatus = Init;
	mutexHtmlWorkThread = std::make_shared< std::mutex >( );
	this->thread = nullptr;
	if( start_thread_run )
		this->startThreadRun = start_thread_run;
	else
		this->startThreadRun = []( ) {
		};
	if( current_thread_run )
		this->currentThreadRun = current_thread_run;
	else
		this->currentThreadRun = []( ) {
		};
	if( finish_thread_run )
		this->finishThreadRun = finish_thread_run;
	else
		this->finishThreadRun = []( ) {
		};

}
cylHtmlTools::HtmlWorkThread::HtmlWorkThread( ) {
	this->workStatus = Init;
	mutexHtmlWorkThread = std::make_shared< std::mutex >( );
	this->thread = nullptr;
	this->startThreadRun = []( ) {
	};
	this->currentThreadRun = []( ) {
	};
	this->finishThreadRun = []( ) {
	};
}
cylHtmlTools::HtmlWorkThread::~HtmlWorkThread( ) {
}
bool cylHtmlTools::HtmlWorkThread::reInit( const std::function< void( ) > &start_thread_run, const std::function< void( ) > &current_thread_run, const std::function< void( ) > &finish_thread_run ) {
	if( isRun( ) )
		return false;
	this->workStatus = Init;
	if( start_thread_run )
		this->startThreadRun = start_thread_run;
	else
		this->startThreadRun = []( ) {
		};
	if( current_thread_run )
		this->currentThreadRun = current_thread_run;
	else
		this->currentThreadRun = []( ) {
		};
	if( finish_thread_run )
		this->finishThreadRun = finish_thread_run;
	else
		this->finishThreadRun = []( ) {
		};
	return true;
}
bool cylHtmlTools::HtmlWorkThread::isRun( ) const {
	mutexHtmlWorkThread->lock( );
	bool result = workStatus == Run;
	mutexHtmlWorkThread->unlock( );
	return result;
}
bool cylHtmlTools::HtmlWorkThread::isFinish( ) const {
	mutexHtmlWorkThread->lock( );
	bool result = workStatus == Finish;
	mutexHtmlWorkThread->unlock( );
	return result;
}
bool cylHtmlTools::HtmlWorkThread::isInit( ) const {
	mutexHtmlWorkThread->lock( );
	bool result = workStatus == Init;
	mutexHtmlWorkThread->unlock( );
	return result;
}
bool cylHtmlTools::HtmlWorkThread::isNone( ) const {
	mutexHtmlWorkThread->lock( );
	bool result = workStatus == None;
	mutexHtmlWorkThread->unlock( );
	return result;
}
void cylHtmlTools::HtmlWorkThread::start( ) {
	this->mutexHtmlWorkThread->lock( );
	if( this->thread )
		delete this->thread;
	this->mutexHtmlWorkThread->unlock( );

	this->mutexHtmlWorkThread->lock( );
	this->workStatus = Run;
	this->thread = new std::thread( [this]( ) {
		if( this->startThreadRun )
			this->startThreadRun( );
		if( this->currentThreadRun )
			this->currentThreadRun( );
		if( this->finishThreadRun )
			this->finishThreadRun( );
		this->mutexHtmlWorkThread->lock( );
		this->workStatus = Finish;
		this->mutexHtmlWorkThread->unlock( );
	} );
	this->mutexHtmlWorkThread->unlock( );
}
void cylHtmlTools::HtmlWorkThread::wait( ) const {
	if( isRun( ) )
		thread->join( );
}
std::function< void( ) > cylHtmlTools::HtmlWorkThread::getStartThreadRun( ) const { return startThreadRun; }
void cylHtmlTools::HtmlWorkThread::setStartThreadRun( const std::function< void( ) > &start_thread_run ) {
	startThreadRun = start_thread_run;
}
std::function< void( ) > cylHtmlTools::HtmlWorkThread::getCurrentThreadRun( ) const { return currentThreadRun; }
void cylHtmlTools::HtmlWorkThread::setCurrentThreadRun( const std::function< void( ) > &current_thread_run ) {
	currentThreadRun = current_thread_run;
}
std::function< void( ) > cylHtmlTools::HtmlWorkThread::getFinishThreadRun( ) const { return finishThreadRun; }
void cylHtmlTools::HtmlWorkThread::setFinishThreadRun( const std::function< void( ) > &finish_thread_run ) {
	finishThreadRun = finish_thread_run;
}
