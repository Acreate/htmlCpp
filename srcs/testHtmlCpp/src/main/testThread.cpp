#include "testThread.h"
#include <codecvt>

#include "../macro/cmake_to_c_cpp_header_env.h"
#include <fstream>
#include <htmls/HtmlNode/HtmlNode.h>
#include <htmls/htmlDoc/HtmlDoc.h>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

#include "htmls/htmlTools/XPath/XPath.h"
#include <htmls/htmlTools/HtmlWorkThread/HtmlWorkThread.h>
#include <htmls/htmlTools/XDirAttribute/XDirAttribute.h>
#include <htmls/htmlTools/XDir/XDir.h>
#include "htmlString/HtmlStringTools.h"

int testThread( std::locale locale ) {
	testHtmlThreadNull( );
	testHtmlThread( );
	testHtmlThreadSharedNull( );
	testHtmlThreadShared( );
	return 4;
}
void testHtmlThreadNull( ) {
	std::cout << u8"testHtmlThreadNull ��ʼ�����߳̿��߳�: " << std::endl;

	cylHtmlTools::HtmlWorkThread< int * > thread;
	thread.start( );
	while( thread.isRun( ) ) {
		std::cout << u8"�ȴ��߳̽���" << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << u8"���߳��Ѿ�ʵ����ɣ����ڽ�����ֵУ�����" << std::endl;
}
void testHtmlThread( ) {
	int coun = 22;
	std::cout << u8"testHtmlThread ��ʼ�����̹߳��� :(׼������) : " << coun << std::endl;
	cylHtmlTools::HtmlWorkThread< int * >::Start_Thread_Run startThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"��ʼһ���߳� :(��������) : " << *data << std::endl;
	};

	// ���Իص�
	startThreadRun( nullptr, nullptr, nullptr, nullptr, &coun );

	cylHtmlTools::HtmlWorkThread< int * >::Current_Thread_Run currentThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data, const time_t *startTime ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"�߳����ڹ��� :(��������) : " << *data << std::endl;
		*data = 777;
		std::cout << u8"�߳����ڹ��� :(�ı�����) : " << *data << std::endl;
	};
	// ���Իص�
	currentThreadRun( nullptr, nullptr, nullptr, nullptr, &coun, nullptr );

	cylHtmlTools::HtmlWorkThread< int * >::Finish_Thread_Run finishThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"����һ���߳� :(��������) : " << *data << std::endl;
	};

	// ���Իص�
	finishThreadRun( nullptr, nullptr, nullptr, nullptr, &coun );

	coun = 22;
	std::cout << u8"���ò�������(" << coun << ")" << std::endl;
	cylHtmlTools::HtmlWorkThread< int * > thread( startThreadRun, currentThreadRun, finishThreadRun, &coun );
	thread.start( );
	while( thread.isRun( ) ) {
		std::cout << u8"�ȴ��߳̽���" << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << u8"�����Ѿ�ʵ����ɣ����ڽ�����ֵУ����� : " << coun << std::endl;
}
void testHtmlThreadSharedNull( ) {
	std::cout << u8"testHtmlThreadSharedNull ��ʼ���Կ��̹߳���" << std::endl;

	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > thread;
	thread.start( );
	while( thread.isRun( ) ) {
		std::cout << u8"�ȴ��߳̽���" << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << u8"���̹߳����Ѿ�ʵ����ɣ����ڽ�����ֵУ����� " << std::endl;
}
void testHtmlThreadShared( ) {
	std::shared_ptr< int > coun( std::make_shared< int >( 22 ) );
	std::cout << u8"testHtmlThreadShared ��ʼ�����̹߳��� :(׼������) : " << *coun << std::endl;
	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > >::Start_Thread_Run startThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, std::shared_ptr< int > &data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"��ʼһ���߳� :(��������) : " << *data << std::endl;
	};

	// ���Իص�
	startThreadRun( nullptr, nullptr, nullptr, nullptr, coun );

	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > >::Current_Thread_Run currentThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, std::shared_ptr< int > &data, const time_t *startTime ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"�߳����ڹ��� :(��������) : " << *data << std::endl;
		*data = 777;
		std::cout << u8"�߳����ڹ��� :(�ı�����) : " << *data << std::endl;
	};
	// ���Իص�
	currentThreadRun( nullptr, nullptr, nullptr, nullptr, coun, nullptr );

	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > >::Finish_Thread_Run finishThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, std::shared_ptr< int > &data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"����һ���߳� :(��������) : " << *data << std::endl;
	};

	// ���Իص�
	finishThreadRun( nullptr, nullptr, nullptr, nullptr, coun );

	*coun = 22;
	std::cout << u8"���ò�������(" << *coun << ")" << std::endl;
	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > thread( startThreadRun, currentThreadRun, finishThreadRun, coun );
	thread.start( );
	while( thread.isRun( ) ) {
		std::cout << u8"�ȴ��߳̽���" << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << u8"�����Ѿ�ʵ����ɣ����ڽ�����ֵУ����� : " << *coun << std::endl;
}
