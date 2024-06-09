#include "HtmlWorkThreadPool.h"


cylHtmlTools::HtmlWorkThreadPool::HtmlWorkThreadPool( ) {
	this->workStatus = HtmlWorkThread::Init;
	mutexHtmlWorkThread = new std::mutex;
	workCount = 0;
	currentWorkThread = 0;
}
cylHtmlTools::HtmlWorkThreadPool::~HtmlWorkThreadPool( ) {
	delete mutexHtmlWorkThread;
}
bool cylHtmlTools::HtmlWorkThreadPool::isRun( ) const {
	mutexHtmlWorkThread->lock( );
	bool result = workStatus == HtmlWorkThread::Run;
	mutexHtmlWorkThread->unlock( );
	return result;
}
bool cylHtmlTools::HtmlWorkThreadPool::isFinish( ) const {
	mutexHtmlWorkThread->lock( );
	bool result = workStatus == HtmlWorkThread::Finish;
	mutexHtmlWorkThread->unlock( );
	return result;
}
bool cylHtmlTools::HtmlWorkThreadPool::isInit( ) const {
	mutexHtmlWorkThread->lock( );
	bool result = workStatus == HtmlWorkThread::Init;
	mutexHtmlWorkThread->unlock( );
	return result;
}
bool cylHtmlTools::HtmlWorkThreadPool::isNone( ) const {
	mutexHtmlWorkThread->lock( );
	bool result = workStatus == HtmlWorkThread::None;
	mutexHtmlWorkThread->unlock( );
	return result;
}
bool cylHtmlTools::HtmlWorkThreadPool::appendWork( const HtmlWorkThread::TThreadCall &work ) {
	if( isRun( ) )
		return false;
	mutexHtmlWorkThread->lock( );
	works.emplace_back( work );
	mutexHtmlWorkThread->unlock( );
	return true;
}
void cylHtmlTools::HtmlWorkThreadPool::start( const cylHtmlTools::HtmlWorkThreadPool::TThreadCall &function_call ) {
	if( workCount == 0 )
		start( 8, function_call );
	else
		start( workCount, function_call );
}
void cylHtmlTools::HtmlWorkThreadPool::start( const size_t work_count, const cylHtmlTools::HtmlWorkThreadPool::TThreadCall &function_call ) {
	if( this->workStatus == HtmlWorkThread::Run )
		return;
	workCount = work_count;
	taskDistributeThread.setCurrentThreadRun( [this]( HtmlWorkThread * ) {

		this->mutexHtmlWorkThread->lock( );
		// 清理任务
		if( workThreads.size( ) > 0 ) {
			for( auto thread : workThreads ) {
				if( !thread->isFinish( ) )
					thread->wait( );
				delete thread;
			}
			workThreads.clear( );
		}
		currentWorkThread = 0;
		this->mutexHtmlWorkThread->unlock( );
		size_t size = works.size( ); // 统计待工作队列大小
		// 初始化工作线程
		for( ; currentWorkThread < size; ) {
			if( currentWorkThread == workCount )
				break;
			auto currentCheckThread = new HtmlWorkThread;
			currentCheckThread->setCurrentThreadRun( works.at( 0 ) );
			currentCheckThread->setFinishThreadRun( [&]( HtmlWorkThread * ) {
				this->mutexHtmlWorkThread->lock( );
				--currentWorkThread;
				this->mutexHtmlWorkThread->unlock( );
			} );
			workThreads.emplace_back( currentCheckThread );
			this->mutexHtmlWorkThread->lock( );
			++currentWorkThread;
			this->mutexHtmlWorkThread->unlock( );
			works.erase( works.begin( ) );
		}
		for( auto thread : workThreads )
			thread->start( );
		do {
			size = works.size( ); // 统计待工作队列大小
			if( size == 0 )
				break;
			std::vector< HtmlWorkThread * > workBuffThreads; // 当前工作线程
			if( currentWorkThread < workCount && size != 0 ) { // 数量不足时，需要填充任务
				auto threadsIterator = workThreads.begin( );
				auto threadsEnd = workThreads.end( );
				for( ; threadsIterator != threadsEnd && size != 0; ++threadsIterator ) { // 找一个空闲线程进行工作
					auto currentCheckThread = *threadsIterator;
					if( currentCheckThread->isFinish( ) ) {
						currentCheckThread->setCurrentThreadRun( works.at( 0 ) );
						currentCheckThread->setFinishThreadRun( [&]( HtmlWorkThread * ) {
							this->mutexHtmlWorkThread->lock( );
							--currentWorkThread;
							this->mutexHtmlWorkThread->unlock( );
						} );
						this->mutexHtmlWorkThread->lock( );
						works.erase( works.begin( ) );
						--size;
						++currentWorkThread;
						this->mutexHtmlWorkThread->unlock( );
						workBuffThreads.emplace_back( currentCheckThread );
					}
				}
			}
			for( auto thread : workBuffThreads )
				thread->start( );
		} while( true );
		auto threadsIterator = workThreads.begin( );
		auto threadsEnd = workThreads.end( );
		for( ; threadsIterator != threadsEnd; ++threadsIterator ) {
			HtmlWorkThread *htmlWorkThread = *threadsIterator;
			htmlWorkThread->wait( );
			delete htmlWorkThread;
		}
		workThreads.clear( );
	} );

	userCallThread.setCurrentThreadRun( [function_call,this]( HtmlWorkThread * ) {
		while( true ) {
			this->mutexHtmlWorkThread->lock( );
			size_t size = works.size( );
			this->mutexHtmlWorkThread->unlock( );
			if( function_call )
				function_call( this, size, currentWorkThread );
			if( size == 0 )
				break;
		}
	} );

	taskDistributeThread.start( );
	userCallThread.start( );
}
void cylHtmlTools::HtmlWorkThreadPool::waiteOverJob( ) {
	if( taskDistributeThread.isInit( ) ) // 处理线程还没开始工作，则返回
		return;
	while( !taskDistributeThread.isFinish( ) ); // 处理线程正在工作，则停留
	while( !userCallThread.isFinish( ) ); // 用户线程正在工作，则停留
	mutexHtmlWorkThread->lock( );
	workStatus = HtmlWorkThread::Finish; // 任务完成
	mutexHtmlWorkThread->unlock( );
}
