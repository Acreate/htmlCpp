#ifndef HTMLWORKTHREAD_CPP_IMPL_H_H_HEAD__FILE__
#define HTMLWORKTHREAD_CPP_IMPL_H_H_HEAD__FILE__
#pragma once
#include "HtmlWorkThread.h"
#include <mutex>
#include <thread>

template< class TDataType >
cylHtmlTools::HtmlWorkThread< TDataType * >::HtmlWorkThread( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, TDataType *data ) {
	this->workStatus = Init;
	mutexHtmlWorkThread = new std::mutex( );
	mutexStdThread = new std::mutex( );
	this->endTime = nullptr;
	this->startTime = nullptr;
	this->startThreadRun = start_thread_run;
	this->currentThreadRun = current_thread_run;
	this->finishThreadRun = finish_thread_run;
	this->data = data;
}
template< class TDataType >
cylHtmlTools::HtmlWorkThread< TDataType * >::~HtmlWorkThread( ) {
	delete mutexHtmlWorkThread;
	delete mutexStdThread;
	if( thread ) {
		thread->join( );
		delete thread;
	}
}
template< class TDataType >
bool cylHtmlTools::HtmlWorkThread< TDataType * >::reInit( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, TDataType *data ) {
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
template< class TDataType >
void cylHtmlTools::HtmlWorkThread< TDataType * >::start( ) {
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
template< class TDataType >
bool cylHtmlTools::HtmlWorkThread< TDataType * >::isRun( ) const {
	this->mutexHtmlWorkThread->lock( );
	bool cond = this->workStatus == Run;
	this->mutexHtmlWorkThread->unlock( );
	return cond;
}
template< class TDataType >
bool cylHtmlTools::HtmlWorkThread< TDataType * >::isFinish( ) const {
	this->mutexHtmlWorkThread->lock( );
	bool cond = this->workStatus == Finish;
	this->mutexHtmlWorkThread->unlock( );
	return cond;
}
template< class TDataType >
time_t cylHtmlTools::HtmlWorkThread< TDataType * >::wait( ) {
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


template< class TDataType >
cylHtmlTools::HtmlWorkThread< std::shared_ptr< TDataType > >::HtmlWorkThread( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, std::shared_ptr< TDataType > &data ) {
	this->workStatus = Init;
	mutexHtmlWorkThread = new std::mutex( );
	mutexStdThread = new std::mutex( );
	this->endTime = nullptr;
	this->startTime = nullptr;
	this->startThreadRun = start_thread_run;
	this->currentThreadRun = current_thread_run;
	this->finishThreadRun = finish_thread_run;
	this->data = data;
}
template< class TDataType >
cylHtmlTools::HtmlWorkThread< std::shared_ptr< TDataType > >::~HtmlWorkThread( ) {
	delete mutexHtmlWorkThread;
	delete mutexStdThread;
	if( thread ) {
		thread->join( );
		delete thread;
	}
}
template< class TDataType >
bool cylHtmlTools::HtmlWorkThread< std::shared_ptr< TDataType > >::reInit( const Start_Thread_Run &start_thread_run, const Current_Thread_Run &current_thread_run, const Finish_Thread_Run &finish_thread_run, std::shared_ptr< TDataType > &data ) {
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
template< class TDataType >
void cylHtmlTools::HtmlWorkThread< std::shared_ptr< TDataType > >::start( ) {
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
template< class TDataType >
bool cylHtmlTools::HtmlWorkThread< std::shared_ptr< TDataType > >::isRun( ) const {
	this->mutexHtmlWorkThread->lock( );
	bool cond = this->workStatus == Run;
	this->mutexHtmlWorkThread->unlock( );
	return cond;
}
template< class TDataType >
bool cylHtmlTools::HtmlWorkThread< std::shared_ptr< TDataType > >::isFinish( ) const {
	this->mutexHtmlWorkThread->lock( );
	bool cond = this->workStatus == Finish;
	this->mutexHtmlWorkThread->unlock( );
	return cond;
}
template< class TDataType >
time_t cylHtmlTools::HtmlWorkThread< std::shared_ptr< TDataType > >::wait( ) {
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

template class cylHtmlTools::HtmlWorkThread< void * >;

template class cylHtmlTools::HtmlWorkThread< char * >;
template class cylHtmlTools::HtmlWorkThread< short * >;
template class cylHtmlTools::HtmlWorkThread< int * >;
template class cylHtmlTools::HtmlWorkThread< long * >;
template class cylHtmlTools::HtmlWorkThread< long long * >;

template class cylHtmlTools::HtmlWorkThread< unsigned char * >;
template class cylHtmlTools::HtmlWorkThread< unsigned short * >;
template class cylHtmlTools::HtmlWorkThread< unsigned int * >;
template class cylHtmlTools::HtmlWorkThread< unsigned long * >;
template class cylHtmlTools::HtmlWorkThread< unsigned long long * >;

template class cylHtmlTools::HtmlWorkThread< double * >;
template class cylHtmlTools::HtmlWorkThread< float * >;


template class cylHtmlTools::HtmlWorkThread< cylHtmlTools::HtmlString * >;
template class cylHtmlTools::HtmlWorkThread< cylHtmlTools::HtmlNode * >;
template class cylHtmlTools::HtmlWorkThread< cylHtmlTools::HtmlDoc * >;
template class cylHtmlTools::HtmlWorkThread< cylHtmlTools::XDir * >;
template class cylHtmlTools::HtmlWorkThread< cylHtmlTools::XDirAttribute * >;
template class cylHtmlTools::HtmlWorkThread< cylHtmlTools::XPath * >;


template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< char > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< short > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< long > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< long long > >;

template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< unsigned char > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< unsigned short > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< unsigned int > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< unsigned long > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< unsigned long long > >;

template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< double > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< float > >;


template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< cylHtmlTools::HtmlString > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< cylHtmlTools::HtmlNode > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< cylHtmlTools::HtmlDoc > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< cylHtmlTools::XDir > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< cylHtmlTools::XDirAttribute > >;
template class cylHtmlTools::HtmlWorkThread< std::shared_ptr< cylHtmlTools::XPath > >;


#endif // HTMLWORKTHREAD_CPP_IMPL_H_H_HEAD__FILE__
