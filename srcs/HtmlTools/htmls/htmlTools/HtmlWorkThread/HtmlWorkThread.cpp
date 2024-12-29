#include "HtmlWorkThread.h"


cylHtmlTools::HtmlWorkThread::HtmlWorkThread( const cylHtmlTools::HtmlWorkThread::TThreadCall &start_thread_run, const cylHtmlTools::HtmlWorkThread::TThreadCall &current_thread_run, const cylHtmlTools::HtmlWorkThread::TThreadCall &finish_thread_run ) {
	this->workStatus = Init;
	mutexHtmlWorkThread = std::make_shared< std::mutex >( );
	this->thread = nullptr;
	if( start_thread_run )
		this->startThreadRun = start_thread_run;
	else
		this->startThreadRun = []( HtmlWorkThread * ) {
		};
	if( current_thread_run )
		this->currentThreadRun = current_thread_run;
	else
		this->currentThreadRun = []( HtmlWorkThread * ) {
		};
	if( finish_thread_run )
		this->finishThreadRun = finish_thread_run;
	else
		this->finishThreadRun = []( HtmlWorkThread * ) {
		};

}
cylHtmlTools::HtmlWorkThread::HtmlWorkThread( ) {
	this->workStatus = Init;
	mutexHtmlWorkThread = std::make_shared< std::mutex >( );
	this->thread = nullptr;
	this->startThreadRun = []( HtmlWorkThread * ) {
	};
	this->currentThreadRun = []( HtmlWorkThread * ) {
	};
	this->finishThreadRun = []( HtmlWorkThread * ) {
	};
}
cylHtmlTools::HtmlWorkThread::~HtmlWorkThread( ) {
	if( this->thread ) {
		if( this->thread->joinable( ) )
			this->thread->join( );
		delete this->thread;
	}
}
bool cylHtmlTools::HtmlWorkThread::reInit( const cylHtmlTools::HtmlWorkThread::TThreadCall &start_thread_run, const cylHtmlTools::HtmlWorkThread::TThreadCall &current_thread_run, const cylHtmlTools::HtmlWorkThread::TThreadCall &finish_thread_run ) {
	if( isRun( ) )
		return false;
	this->workStatus = Init;
	if( start_thread_run )
		this->startThreadRun = start_thread_run;
	else
		this->startThreadRun = []( HtmlWorkThread * ) {
		};
	if( current_thread_run )
		this->currentThreadRun = current_thread_run;
	else
		this->currentThreadRun = []( HtmlWorkThread * ) {
		};
	if( finish_thread_run )
		this->finishThreadRun = finish_thread_run;
	else
		this->finishThreadRun = []( HtmlWorkThread * ) {
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
	if( result && this->thread->joinable( ) )
		this->thread->join( );
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
	if( isRun( ) )
		return;
	this->mutexHtmlWorkThread->lock( );
	if( this->thread ) {
		if( this->thread->joinable( ) )
			this->thread->join( );
		delete this->thread;
	}
	this->mutexHtmlWorkThread->unlock( );

	this->mutexHtmlWorkThread->lock( );
	this->workStatus = Run;
	this->thread = new std::thread( [this]( ) {
		if( this->startThreadRun )
			this->startThreadRun( this );

		if( this->currentThreadRun )
			this->currentThreadRun( this );

		if( this->finishThreadRun )
			this->finishThreadRun( this );
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
cylHtmlTools::HtmlWorkThread::TThreadCall cylHtmlTools::HtmlWorkThread::getStartThreadRun( ) const { return startThreadRun; }
void cylHtmlTools::HtmlWorkThread::setStartThreadRun( const cylHtmlTools::HtmlWorkThread::TThreadCall &start_thread_run ) {
	startThreadRun = start_thread_run;
}
cylHtmlTools::HtmlWorkThread::TThreadCall cylHtmlTools::HtmlWorkThread::getCurrentThreadRun( ) const { return currentThreadRun; }
void cylHtmlTools::HtmlWorkThread::setCurrentThreadRun( const cylHtmlTools::HtmlWorkThread::TThreadCall &current_thread_run ) {
	currentThreadRun = current_thread_run;
}
cylHtmlTools::HtmlWorkThread::TThreadCall cylHtmlTools::HtmlWorkThread::getFinishThreadRun( ) const { return finishThreadRun; }
void cylHtmlTools::HtmlWorkThread::setFinishThreadRun( const cylHtmlTools::HtmlWorkThread::TThreadCall &finish_thread_run ) {
	finishThreadRun = finish_thread_run;
}
