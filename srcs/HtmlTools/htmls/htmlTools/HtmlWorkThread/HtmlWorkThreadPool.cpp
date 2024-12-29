#include "HtmlWorkThreadPool.h"


cylHtmlTools::HtmlWorkThreadPool::HtmlWorkThreadPool( ) {
	this->workStatus = HtmlWorkThread::Init;
	mutexHtmlWorkThread = new std::mutex;
	workCount = 0;
	currentWorkThread = 0;
	idleTimeCall = nullptr;
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

inline void overThreas( std::vector< cylHtmlTools::HtmlWorkThread * > &workThreads ) {
	do {
		auto threadsIterator = workThreads.begin( );
		auto threadsEnd = workThreads.end( );
		if( threadsIterator == threadsEnd )
			break; // 没有元素，则跳出最外套循环
		do {
			cylHtmlTools::HtmlWorkThread *htmlWorkThread = *threadsIterator;
			if( htmlWorkThread->isFinish( ) ) {
				workThreads.erase( threadsIterator );
				htmlWorkThread->wait( );
				delete htmlWorkThread;
				break; // 重新遍历
			}
			++threadsIterator;
		} while( threadsIterator != threadsEnd );
	} while( true );
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
		size_t size = works.size( ); // 统计待工作队列大小
		this->mutexHtmlWorkThread->unlock( );
		// 初始化工作线程
		while( currentWorkThread < size ) {
			if( currentWorkThread == workCount )
				break;
			auto currentCheckThread = new HtmlWorkThread;
			this->mutexHtmlWorkThread->lock( );
			currentCheckThread->setCurrentThreadRun( works.at( 0 ) );
			this->mutexHtmlWorkThread->unlock( );
			currentCheckThread->setFinishThreadRun( [&]( HtmlWorkThread * ) {
				this->mutexHtmlWorkThread->lock( );
				--currentWorkThread;
				this->mutexHtmlWorkThread->unlock( );
			} );
			workThreads.emplace_back( currentCheckThread );
			this->mutexHtmlWorkThread->lock( );
			++currentWorkThread;
			works.erase( works.begin( ) );
			this->mutexHtmlWorkThread->unlock( );
		}
		for( auto thread : workThreads )
			thread->start( );
		do {
			this->mutexHtmlWorkThread->lock( );
			size = works.size( ); // 统计待工作队列大小
			this->mutexHtmlWorkThread->unlock( );
			if( size == 0 )
				break;
			std::vector< HtmlWorkThread * > workBuffThreads; // 临时存储指针，然后统一开始
			if( currentWorkThread < workCount && size != 0 ) { // 数量不足时，需要填充任务
				auto threadsIterator = workThreads.begin( );
				auto threadsEnd = workThreads.end( );
				for( ; threadsIterator != threadsEnd && size != 0; ++threadsIterator ) { // 找一个空闲线程进行工作
					auto currentCheckThread = *threadsIterator;
					if( currentCheckThread->isFinish( ) ) {
						this->mutexHtmlWorkThread->lock( );
						currentCheckThread->setCurrentThreadRun( works.at( 0 ) );
						currentCheckThread->setFinishThreadRun( [&]( HtmlWorkThread * ) {
							this->mutexHtmlWorkThread->lock( );
							--currentWorkThread;
							this->mutexHtmlWorkThread->unlock( );
						} );
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
		overThreas( workThreads );
	} );
	if( function_call )
		userCallThread.setCurrentThreadRun( [function_call,this]( HtmlWorkThread * ) {
			auto makeTimePoint = std::chrono::system_clock::now( );

			this->mutexHtmlWorkThread->lock( );
			size_t size = works.size( );
			this->mutexHtmlWorkThread->unlock( );
			function_call( this, size, currentWorkThread );
			while( size != 0 || currentWorkThread != 0 ) {
				auto newTimePoint = std::chrono::system_clock::now( );
				auto sepMill = std::chrono::duration_cast< std::chrono::milliseconds >( newTimePoint - makeTimePoint );
				if( sepMill < callSepMilliseconds ) {
					std::this_thread::sleep_for( idleTimeCallWaiteSec );
					this->mutexHtmlWorkThread->lock( );
					size = works.size( );
					this->mutexHtmlWorkThread->unlock( );
					continue;
				}
				makeTimePoint = newTimePoint;
				this->mutexHtmlWorkThread->lock( );
				size = works.size( );
				this->mutexHtmlWorkThread->unlock( );
				function_call( this, size, currentWorkThread );

			}
		} );
	else if( this->idleTimeCall )
		userCallThread.setCurrentThreadRun( [this]( HtmlWorkThread * ) {
			auto makeTimePoint = std::chrono::system_clock::now( );
			this->mutexHtmlWorkThread->lock( );
			size_t size = works.size( );
			this->mutexHtmlWorkThread->unlock( );
			( *idleTimeCall )( this, size, currentWorkThread );
			while( size != 0 || currentWorkThread != 0 ) {
				auto newTimePoint = std::chrono::system_clock::now( );
				auto sepMill = std::chrono::duration_cast< std::chrono::milliseconds >( newTimePoint - makeTimePoint );
				if( sepMill < callSepMilliseconds ) {
					std::this_thread::sleep_for( idleTimeCallWaiteSec );
					this->mutexHtmlWorkThread->lock( );
					size = works.size( );
					this->mutexHtmlWorkThread->unlock( );
					continue;
				}
				makeTimePoint = newTimePoint;
				this->mutexHtmlWorkThread->lock( );
				size = works.size( );
				this->mutexHtmlWorkThread->unlock( );
				( *idleTimeCall )( this, size, currentWorkThread );

			}
		} );
	else
		userCallThread.setCurrentThreadRun( [this]( HtmlWorkThread * ) {
			auto makeTimePoint = std::chrono::system_clock::now( );
			this->mutexHtmlWorkThread->lock( );
			size_t size = works.size( );
			this->mutexHtmlWorkThread->unlock( );
			while( size != 0 || currentWorkThread != 0 ) {
				auto newTimePoint = std::chrono::system_clock::now( );
				auto sepMill = std::chrono::duration_cast< std::chrono::milliseconds >( newTimePoint - makeTimePoint );
				if( sepMill < callSepMilliseconds ) {
					std::this_thread::sleep_for( idleTimeCallWaiteSec );
					this->mutexHtmlWorkThread->lock( );
					size = works.size( );
					this->mutexHtmlWorkThread->unlock( );
					continue;
				}
				makeTimePoint = newTimePoint;
				this->mutexHtmlWorkThread->lock( );
				size = works.size( );
				this->mutexHtmlWorkThread->unlock( );
			}
		} );
	this->workStatus = HtmlWorkThread::Run;
	taskDistributeThread.start( );
	userCallThread.start( );
}
void cylHtmlTools::HtmlWorkThreadPool::waiteOverJob( ) {
	while( !isOverJob( ) );
}

bool cylHtmlTools::HtmlWorkThreadPool::isOverJob( ) {
	if( taskDistributeThread.isInit( ) ) // 处理线程还没开始工作，则返回
		return false;
	if( !taskDistributeThread.isFinish( ) )return false; // 处理线程正在工作，则停留
	if( !userCallThread.isFinish( ) )return false; // 用户线程正在工作，则停留
	mutexHtmlWorkThread->lock( );
	workStatus = HtmlWorkThread::Finish; // 任务完成
	mutexHtmlWorkThread->unlock( );
	return true;
}
