﻿#include <codecvt>

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

/// <summary>
/// 根查找
/// </summary>
/// <param name="stringstream"></param>
/// <param name="htmlDoc"></param>
/// <param name="xpath"></param>
/// <returns></returns>
bool getValue( std::wstringstream &stringstream, cylHtmlTools::HtmlDoc_Shared htmlDoc, cylHtmlTools::XPath &xpath ) {
	auto htmlNodeRoots = htmlDoc->getHtmlNodeRoots( );
	auto vectorHtmlNodeSPtrShared = xpath.buider( htmlNodeRoots );
	if( !vectorHtmlNodeSPtrShared )
		return false;
	stringstream << xpath.getHtmlString( ) << L":\n";
	stringstream << L"\t" << L"======== htmlDoc ===============" << std::endl;
	for( auto &node : *vectorHtmlNodeSPtrShared ) {
		auto name = *node->getNodeName( );
		auto contentText = *node->getNodeContentText( );
		auto content = *node->getNodeContent( );
		auto path = *node->getPath( );
		stringstream << L"\t" << L"=======================" << std::endl;
		stringstream << L"\t" << L"找到 [" << name << L"]:" << path << std::endl;
		stringstream << L"\t" << L"<<<<<<<<<<" L"\t" L"nodeText" L"\t" L"<<<<<<<<<<" << std::endl;
		stringstream << L"\t" << contentText << std::endl;
		stringstream << L"\t" << L"<<<<<<<<<<" L"\t" L"nodeContent" L"\t" L"<<<<<<<<<<" << std::endl;
		stringstream << L"\t" << content << std::endl;
		stringstream << L"\t" << L"=======================" << std::endl;
	}
	stringstream << std::endl << std::endl;
	return true;
}
/// <summary>
/// 节点查找
/// </summary>
/// <param name="stringstream"></param>
/// <param name="html_nodes"></param>
/// <param name="xpath"></param>
/// <returns></returns>
bool getValue( std::wstringstream &stringstream, cylHtmlTools::Vector_HtmlNodeSPtr_Shared &html_nodes, cylHtmlTools::XPath &xpath ) {
	auto vectorHtmlNodeSPtrShared = xpath.buider( html_nodes );
	if( !vectorHtmlNodeSPtrShared )
		return false;
	stringstream << xpath.getHtmlString( ) << L":\n";
	for( auto &htmlNode : *html_nodes ) {
		auto name = *htmlNode->getNodeName( );
		auto content = *htmlNode->getNodeContent( );
		auto path = *htmlNode->getPath( );
		stringstream << L"\t" << L"------------------------" << std::endl;
		stringstream << L"\t" << L"遍历 [" << name << L"]:" << path << std::endl;
		stringstream << L"\t" << L"++++++++++" L"\t" L"nodeContent" L"\t" L"++++++++++" << std::endl;
		stringstream << L"\t" << content << std::endl;
		stringstream << L"\t" << L"------------------------" << std::endl;
	}
	for( auto &node : *vectorHtmlNodeSPtrShared ) {
		auto name = *node->getNodeName( );
		auto contentText = *node->getNodeContentText( );
		auto content = *node->getNodeContent( );
		auto path = *node->getPath( );
		stringstream << L"\t" L"\t" << L"=======================" << std::endl;
		stringstream << L"\t" L"\t" << L"找到 [" << name << L"]:" << path << std::endl;
		stringstream << L"\t" L"\t" << L"<<<<<<<<<<" L"\t" L"nodeText" L"\t" L"<<<<<<<<<<" << std::endl;
		stringstream << L"\t" L"\t" << contentText << std::endl;
		stringstream << L"\t" L"\t" << L"<<<<<<<<<<" L"\t" L"nodeContent" L"\t" L"<<<<<<<<<<" << std::endl;
		stringstream << L"\t" L"\t" << content << std::endl;
		stringstream << L"\t" L"\t" << L"=======================" << std::endl;
	}
	stringstream << std::endl << std::endl;
	return true;
}

/// <summary>
/// HtmlDoc 案例
/// </summary>
/// <param name="locale">输出编码</param>
/// <returns>非 0 表示异常</returns>
int testHtmlDoc( std::locale locale = std::locale( ) ) {
	std::string fString( u8"%s/%s/%s" );
	char path[ 4096 ]{ 0 };
	int len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Project_Run_bin ).c_str( ), u8"writeFile", u8"www.121ds.cc.html" );
	if( len == 0 )
		return -1;
	std::wifstream ifstream( path, std::ios::binary | std::ios::in );
	if( !ifstream.is_open( ) )
		return -2;
	std::wstringstream stringstream;
	stringstream.imbue( locale );
	do {
		std::wstring getStr;
		auto &getline = std::getline( ifstream, getStr );
		if( getline.eof( ) )
			break;
		if( !getStr.empty( ) )
			stringstream << getStr;
		stringstream << L'\n';
	} while( true );
	std::shared_ptr< std::wstring > htmlContent( new std::wstring( stringstream.str( ) ) );
	ifstream.close( );
	// 写入文件
	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"www.121ds.cc.txt" );
	if( len == 0 )
		return -1;
	std::wofstream ofstream( path, std::ios::binary | std::ios::out | std::ios::trunc );
	if( !ofstream.is_open( ) )
		return -2;
	ofstream << stringstream.str( );
	ofstream.close( );

	// 测试调用
	size_t endIndex = htmlContent->size( ), startIndex = 0;
	auto htmlDoc = cylHtmlTools::HtmlDoc::parse( htmlContent, endIndex, startIndex );


	stringstream = std::wstringstream( );
	htmlDoc->findNodes( [=,&stringstream]( cylHtmlTools::HtmlNode_Shared &node )->bool {
		auto htmlNodeType = node->getNodeType( );
		cylHtmlTools::HtmlString type;
		switch( htmlNodeType ) {
			case cylHtmlTools::Html_Node_Type::DoubleNode :
				type = L"DoubleNode";
				break;
			case cylHtmlTools::Html_Node_Type::SingleNode :
				type = L"SingleNode";
				break;
			case cylHtmlTools::Html_Node_Type::AnnotationNode :
				type = L"AnnotationNode";
				break;
			case cylHtmlTools::Html_Node_Type::None :
				type = L"None";
				break;
		}
		auto nodeName = *node->getNodeName( );
		auto path = *node->getPath( );
		stringstream << nodeName << " : " << type << " : " << path << std::endl;
		return false;
	} );

	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"www.121ds.cc.allNode.txt" );
	ofstream.open( path, std::ios::binary | std::ios::out | std::ios::trunc );
	if( !ofstream.is_open( ) )
		return -2;
	ofstream << stringstream.str( );
	ofstream.close( );


	// 解析族谱关系（父子，兄弟关系）
	htmlDoc->analysisBrotherNode( );
	// 解析所有节点关系 (单独解析可以使用 HtmlStringPairUnorderMap_Shared HtmlNode::analysisAttribute( ))
	htmlDoc->analysisAttributesNode( );

	auto topstr = "↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑";
	stringstream = std::wstringstream( );
	cylHtmlTools::XPath xpath;

	xpath = cylHtmlTools::XPath( L"//body/div/div/div/div[@class='hd']/ul/li/a" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	if( !getValue( stringstream, htmlDoc, xpath ) )
		return -7;
	std::cout << topstr << std::endl;

	auto param( std::make_shared< cylHtmlTools::Vector_HtmlNodeSPtr >( ) );
	auto nodeSPtrShared = htmlDoc->getHtmlNodeRoots( );
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		param->emplace_back( node );
		std::wcout << *node->getPath( ) << std::endl;
	}
	xpath = cylHtmlTools::XPath( L"./html" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"../html" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"/" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div/ul/li/a" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div[@class='hd']/ul/li/a" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div[@class='hd br']/ul/li/a" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div[@class='hd cf']/ul/li/a" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div[@class='hd cf']" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div[@class='hd cf']/div" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div[@class='hd cf']/div/a/img" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, param, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	xpath = cylHtmlTools::XPath( L"div[@class='hd']" );
	std::wcout << std::endl << std::endl << xpath.getHtmlString( ) << std::endl << "\t" "-----------------" << std::endl;
	param->clear( );
	for( auto &node : *nodeSPtrShared ) {
		for( auto &subNode : *node->getChildren( ) )
			param->emplace_back( subNode );
	}
	if( !getValue( stringstream, htmlDoc, xpath ) )
		std::cout << "没有找到" << std::endl;
	std::cout << topstr << std::endl;

	// 写入文件
	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"www.121ds.cc.xpath.txt" );
	if( len == 0 )
		return -1;
	ofstream.open( path, std::ios::binary | std::ios::out | std::ios::trunc );
	if( !ofstream.is_open( ) )
		return -2;

	auto mystr = stringstream.str( );
	ofstream << mystr;
	ofstream.close( );
	return 0;
}
/// <summary>
/// 线程案例
/// </summary>
void testHtmlThreadNull( ) {
	std::cout << u8"testHtmlThreadNull 开始测试线程空线程: " << std::endl;

	cylHtmlTools::HtmlWorkThread< int * > thread;
	thread.start( );
	while( thread.isRun( ) ) {
		std::cout << u8"等待线程结束" << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << u8"空线程已经实现完成，正在进行数值校验输出" << std::endl;
}
/// <summary>
/// 线程案例
/// </summary>
void testHtmlThread( ) {
	int coun = 22;
	std::cout << u8"testHtmlThread 开始测试线程功能 :(准备数据) : " << coun << std::endl;
	cylHtmlTools::HtmlWorkThread< int * >::Start_Thread_Run startThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"开始一个线程 :(发现数据) : " << *data << std::endl;
	};

	// 测试回调
	startThreadRun( nullptr, nullptr, nullptr, nullptr, &coun );

	cylHtmlTools::HtmlWorkThread< int * >::Current_Thread_Run currentThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data, const time_t *startTime ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"线程正在工作 :(发现数据) : " << *data << std::endl;
		*data = 777;
		std::cout << u8"线程正在工作 :(改变数据) : " << *data << std::endl;
	};
	// 测试回调
	currentThreadRun( nullptr, nullptr, nullptr, nullptr, &coun, nullptr );

	cylHtmlTools::HtmlWorkThread< int * >::Finish_Thread_Run finishThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"结束一个线程 :(发现数据) : " << *data << std::endl;
	};

	// 测试回调
	finishThreadRun( nullptr, nullptr, nullptr, nullptr, &coun );

	coun = 22;
	std::cout << u8"重置测试数据(" << coun << ")" << std::endl;
	cylHtmlTools::HtmlWorkThread< int * > thread( startThreadRun, currentThreadRun, finishThreadRun, &coun );
	thread.start( );
	while( thread.isRun( ) ) {
		std::cout << u8"等待线程结束" << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << u8"工作已经实现完成，正在进行数值校验输出 : " << coun << std::endl;
}
/// <summary>
/// 线程案例
/// </summary>
void testHtmlThreadSharedNull( ) {
	std::cout << u8"testHtmlThreadSharedNull 开始测试空线程功能" << std::endl;

	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > thread;
	thread.start( );
	while( thread.isRun( ) ) {
		std::cout << u8"等待线程结束" << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << u8"空线程功能已经实现完成，正在进行数值校验输出 " << std::endl;
}
/// <summary>
/// 线程案例
/// </summary>
void testHtmlThreadShared( ) {
	std::shared_ptr< int > coun( std::make_shared< int >( 22 ) );
	std::cout << u8"testHtmlThreadShared 开始测试线程功能 :(准备数据) : " << *coun << std::endl;
	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > >::Start_Thread_Run startThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, std::shared_ptr< int > &data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"开始一个线程 :(发现数据) : " << *data << std::endl;
	};

	// 测试回调
	startThreadRun( nullptr, nullptr, nullptr, nullptr, coun );

	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > >::Current_Thread_Run currentThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, std::shared_ptr< int > &data, const time_t *startTime ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"线程正在工作 :(发现数据) : " << *data << std::endl;
		*data = 777;
		std::cout << u8"线程正在工作 :(改变数据) : " << *data << std::endl;
	};
	// 测试回调
	currentThreadRun( nullptr, nullptr, nullptr, nullptr, coun, nullptr );

	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > >::Finish_Thread_Run finishThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, std::shared_ptr< int > &data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"结束一个线程 :(发现数据) : " << *data << std::endl;
	};

	// 测试回调
	finishThreadRun( nullptr, nullptr, nullptr, nullptr, coun );

	*coun = 22;
	std::cout << u8"重置测试数据(" << *coun << ")" << std::endl;
	cylHtmlTools::HtmlWorkThread< std::shared_ptr< int > > thread( startThreadRun, currentThreadRun, finishThreadRun, coun );
	thread.start( );
	while( thread.isRun( ) ) {
		std::cout << u8"等待线程结束" << std::endl;
		std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
	}

	std::cout << u8"工作已经实现完成，正在进行数值校验输出 : " << *coun << std::endl;
}

std::wstringstream getFileAllWString( const char *path ) {
	std::wstringstream stringstream;
	std::wifstream ifstream( path, std::ios::binary | std::ios::in );
	if( !ifstream.is_open( ) )
		return stringstream;
	do {
		std::wstring getStr;
		auto &getline = std::getline( ifstream, getStr );
		if( getline.eof( ) )
			break;
		if( !getStr.empty( ) )
			stringstream << getStr;
		stringstream << L'\n';
	} while( true );
	std::shared_ptr< std::wstring > htmlContent( new std::wstring( stringstream.str( ) ) );
	ifstream.close( );
	return stringstream;

}
size_t setFileAllWString( const char *path, const cylHtmlTools::HtmlString &stringstream ) {
	std::wofstream ofstream;
	ofstream.open( path, std::ios::binary | std::ios::out | std::ios::trunc );
	if( !ofstream.is_open( ) )
		return 0;
	auto &&write = ofstream.write( stringstream.c_str( ), stringstream.size( ) );
	return write.tellp( );
}
/// <summary>
/// 测试 XDirAttribute
/// </summary>
/// <param name="test_paremt_name">XDirAttribute 名称</param>
/// <param name="test_paremt_value">XDirAttribute 值</param>
void testXAttribute( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value ) {
	cylHtmlTools::HtmlString_Shared xattributeName(
		std::make_shared< cylHtmlTools::HtmlString >( test_paremt_name )
	);
	cylHtmlTools::HtmlString_Shared xattributeValue(
		std::make_shared< cylHtmlTools::HtmlString >( test_paremt_value )
	);
	cylHtmlTools::XDirAttribute xAttribute( xattributeName->c_str( ), xattributeName->length( ), xattributeValue->c_str( ), xattributeValue->length( ) );
	auto name = xAttribute.getName( );
	std::wcout << L"===============" << std::endl;
	if( name )
		std::wcout << L"找到名称: \"" << *name << '\"' << std::endl;
	else
		std::wcout << L"名称转化错误" << std::endl;
	auto values = xAttribute.getValues( );
	if( values )
		for( auto &valuePtr : *values )
			std::wcout << L"找到值: \"" << *valuePtr << '\"' << std::endl;
	else
		std::wcout << L"值转化错误" << std::endl;

	std::wcout << L"===============" << std::endl;
}
void testXAttribute( const cylHtmlTools::HtmlString &test_paremt ) {
	testXAttribute( test_paremt, test_paremt );
}
/// <summary>
/// 测试 XDir 对象
/// </summary>
/// <param name="test_paremt">生成 XDir 对象的参数</param>
void testXDir( const cylHtmlTools::HtmlString &test_paremt ) {
	cylHtmlTools::XDir xdir( test_paremt );
	auto maps = xdir.getAttributeMaps( );
	auto iterator = maps.begin( );
	auto end = maps.end( );
	std::wcout << L"=============== 测试 XDir 生成对象" << std::endl;
	for( ; iterator != end; ++iterator ) {
		std::wcout << L"获得路径名称:\"" << iterator->first << '\"' << std::endl;
		for( auto &str : *iterator->second ) {
			auto &xdirAttribute = *str;
			auto name = xdirAttribute.getName( );
			if( name )
				std::wcout << '\t' << L"获得属性名称:\"" << *name << '\"' << std::endl;
			auto htmlStringSPtrShared = xdirAttribute.getValues( );
			if( htmlStringSPtrShared )
				for( auto &value : *htmlStringSPtrShared )
					std::wcout << "\t\t" << L"获得属性值:\"" << *value << '\"' << std::endl;
		}
		std::wcout << std::endl;
	}
	std::wcout << L"===============" << std::endl;
}
/// <summary>
/// 测试 XDir 对象的包含功能
/// </summary>
/// <param name="left_param">左生成对象字符串</param>
/// <param name="right_param">右生成对象字符串</param>
void testIncludeXDir( const cylHtmlTools::HtmlString &left_param, const cylHtmlTools::HtmlString &right_param ) {
	cylHtmlTools::XDir leftXdir( left_param );
	cylHtmlTools::XDir rightXdir( right_param );
	std::wcout << L"=============== 测试 xdir 包含功能" << std::endl;
	do {
		auto maps = leftXdir.getAttributeMaps( );
		auto iterator = maps.begin( );
		auto end = maps.end( );
		for( ; iterator != end; ++iterator ) {
			std::wcout << L"leftXdir 获得路径名称:\"" << iterator->first << '\"' << std::endl;
			for( auto &str : *iterator->second ) {
				auto &xdirAttribute = *str;
				auto name = xdirAttribute.getName( );
				if( name )
					std::wcout << '\t' << L"获得属性名称:\"" << *name << '\"' << std::endl;
				auto htmlStringSPtrShared = xdirAttribute.getValues( );
				if( htmlStringSPtrShared )
					for( auto &value : *htmlStringSPtrShared )
						std::wcout << "\t\t" << L"获得属性值:\"" << *value << '\"' << std::endl;
			}
			std::wcout << std::endl;
		}
	} while( false );
	do {
		auto maps = rightXdir.getAttributeMaps( );
		auto iterator = maps.begin( );
		auto end = maps.end( );
		for( ; iterator != end; ++iterator ) {
			std::wcout << L"rightXdir 获得路径名称:\"" << iterator->first << '\"' << std::endl;
			for( auto &str : *iterator->second ) {
				auto &xdirAttribute = *str;
				auto name = xdirAttribute.getName( );
				if( name )
					std::wcout << '\t' << L"获得属性名称:\"" << *name << '\"' << std::endl;
				auto htmlStringSPtrShared = xdirAttribute.getValues( );
				if( htmlStringSPtrShared )
					for( auto &value : *htmlStringSPtrShared )
						std::wcout << "\t\t" << L"获得属性值:\"" << *value << '\"' << std::endl;
			}
			std::wcout << std::endl;
		}
	} while( false );
	bool isInclude = cylHtmlTools::XDir::isLeftXDirIncludeRightXDir( leftXdir, rightXdir );
	std::wcout << L"isLeftXDirIncludeRightXDir : " << ( isInclude ? L"包含" : L"不包含" ) << std::endl;
	std::wcout << L"===============" << std::endl;
}
/// <summary>
/// 测试 XPath 对象
/// </summary>
/// <param name="test_paremt"> xpath 对象的参数</param>
void testXPath( const cylHtmlTools::HtmlString &test_paremt ) {
	cylHtmlTools::XPath xpath( test_paremt );

	std::wcout << L"===============" << std::endl;
	std::wcout << L"获取路径 : \"" << xpath.getHtmlString( ) << '\"' << std::endl;
	auto xDirS = xpath.getXDirs( );
	for( auto &xdir : xDirS ) {
		auto xdirName = xdir->getDirName( );
		if( xdirName.empty( ) )
			std::wcout << L"名称转化错误" << std::endl;
		else
			std::wcout << L"找到目录名称: \"" << xdirName << '\"' << std::endl;
		auto maps = xdir->getAttributeMaps( );
		auto iterator = maps.begin( );
		auto end = maps.end( );
		for( ; iterator != end; ++iterator ) {
			std::wcout << L"获得路径名称:\"" << iterator->first << '\"' << std::endl;
			for( auto &str : *iterator->second ) {
				auto &xdirAttribute = *str;
				auto name = xdirAttribute.getName( );
				if( name )
					std::wcout << '\t' << L"获得属性名称:\"" << *name << '\"' << std::endl;
				auto htmlStringSPtrShared = xdirAttribute.getValues( );
				if( htmlStringSPtrShared )
					for( auto &value : *htmlStringSPtrShared )
						std::wcout << "\t\t" << L"获得属性值:\"" << *value << '\"' << std::endl;
			}
			std::wcout << std::endl;
		}
	}
	std::wcout << L"===============" << std::endl;
}

/// <summary>
/// 测试属性对象是否包含指定属性值
/// </summary>
/// <param name="test_paremt_name">用于构建 xdir 对象的名称参数</param>
/// <param name="test_paremt_value">用于构建 xdir 对象的值参数</param>
/// <param name="value">用于匹配被包含的值列表</param>
void testXAttributeIsIncludeOther( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value, const std::vector< cylHtmlTools::HtmlString > &value ) {
	cylHtmlTools::HtmlString_Shared xattributeName(
		std::make_shared< cylHtmlTools::HtmlString >( test_paremt_name )
	);
	cylHtmlTools::HtmlString_Shared xattributeValue(
		std::make_shared< cylHtmlTools::HtmlString >( test_paremt_value )
	);
	cylHtmlTools::XDirAttribute xAttribute( xattributeName->c_str( ), xattributeName->length( ), xattributeValue->c_str( ), xattributeValue->length( ) );
	auto name = xAttribute.getName( );
	std::wcout << L"=============== 测试 XDirAttribute 包含其他值列表" << std::endl;
	if( name )
		std::wcout << L"\t找到名称: \"" << *name << '\"' << std::endl;
	else
		std::wcout << L"\t名称转化错误" << std::endl;
	auto values = xAttribute.getValues( );
	if( values )
		for( auto &valuePtr : *values )
			std::wcout << L"\t\t找到值: \"" << *valuePtr << '\"' << std::endl;
	else
		std::wcout << L"\t值转化错误" << std::endl;

	std::wcout << L"\t输出值列表 : ";
	auto iterator = value.begin( );
	auto end = value.end( );
	do {
		std::wcout << '\"' << *iterator << '\"';
		++iterator;
		if( iterator == end )
			break;
		std::wcout << L", ";
	} while( true );
	std::wcout << std::endl;

	bool isIncludeOtherXDirAttributeValues = xAttribute.isThisXDirAttributeValuesIncludeOtherXDirAttributeValues( value );
	std::wcout << L"\t查找值列表 : " << ( isIncludeOtherXDirAttributeValues ? L"包含" : L"不包含" ) << std::endl;

	std::wcout << L"===============" << std::endl;
}
/// <summary>
/// 测试属性对象是否包含指定属性值
/// </summary>
/// <param name="test_paremt">用于构建 xdir 对象的名称参数</param>
/// <param name="value">用于匹配被包含的值列表</param>
void testXAttributeIsIncludeOther( const cylHtmlTools::HtmlString &test_paremt, const std::vector< cylHtmlTools::HtmlString > &value ) {
	testXAttributeIsIncludeOther( test_paremt, test_paremt, value );
}
/// <summary>
/// 测试属性对象是否被包含在指定列表对象当中
/// </summary>
/// <param name="test_paremt_name">用于构建 xdir 对象的名称参数</param>
/// <param name="test_paremt_value">用于构建 xdir 对象的值参数</param>
/// <param name="value">用于匹配包含 xdir 对象的值列表</param>
void testXAttributeIsOtherInclude( const cylHtmlTools::HtmlString &test_paremt_name, const cylHtmlTools::HtmlString &test_paremt_value, const std::vector< cylHtmlTools::HtmlString > &value ) {
	cylHtmlTools::HtmlString_Shared xattributeName(
		std::make_shared< cylHtmlTools::HtmlString >( test_paremt_name )
	);
	cylHtmlTools::HtmlString_Shared xattributeValue(
		std::make_shared< cylHtmlTools::HtmlString >( test_paremt_value )
	);
	cylHtmlTools::XDirAttribute xAttribute( xattributeName->c_str( ), xattributeName->length( ), xattributeValue->c_str( ), xattributeValue->length( ) );
	auto name = xAttribute.getName( );
	std::wcout << L"=============== 测试其他包含 XDirAttribute 值列表" << std::endl;
	if( name )
		std::wcout << L"\t找到名称: \"" << *name << '\"' << std::endl;
	else
		std::wcout << L"\t名称转化错误" << std::endl;
	auto values = xAttribute.getValues( );
	if( values )
		for( auto &valuePtr : *values )
			std::wcout << L"\t找到值: \"" << *valuePtr << '\"' << std::endl;
	else
		std::wcout << L"\t值转化错误" << std::endl;

	std::wcout << L"\t输出值列表 : ";
	auto iterator = value.begin( );
	auto end = value.end( );
	do {
		std::wcout << '\"' << *iterator << '\"';
		++iterator;
		if( iterator == end )
			break;
		std::wcout << L", ";
	} while( true );
	std::wcout << std::endl;
	bool isIncludeOtherXDirAttributeValues = xAttribute.isOtherXDirAttributeValuesIncludeThisXDirAttributeValues( value );
	std::wcout << L"\t查找值列表 : " << ( isIncludeOtherXDirAttributeValues ? L"包含" : L"不包含" ) << std::endl;

	std::wcout << L"===============" << std::endl;
}
/// <summary>
/// 测试属性对象是否被包含在指定列表对象当中
/// </summary>
/// <param name="test_paremt">用于构建 xdir 对象的参数</param>
/// <param name="value">用于匹配包含 xdir 对象的值列表</param>
void testXAttributeIsOtherInclude( const cylHtmlTools::HtmlString &test_paremt, const std::vector< cylHtmlTools::HtmlString > &value ) {
	testXAttributeIsOtherInclude( test_paremt, test_paremt, value );

}
void findElements( cylHtmlTools::HtmlDoc_Shared htmlDoc, const cylHtmlTools::HtmlString &lrDivIdSitebox ) {
	std::wcout << L"↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓ \t测试 xptah 查找" << std::endl;
	std::wcout << L"xpath 查找 : \"" << lrDivIdSitebox << '\"' << std::endl;
	cylHtmlTools::XPath xpath( lrDivIdSitebox );
	auto htmlNodeSPtrShared = htmlDoc->xpathRootNodes( xpath );
	if( !htmlNodeSPtrShared ) {
		std::wcout << L"找不到任何元素" << std::endl;
	} else {
		for( auto &node : *htmlNodeSPtrShared ) {
			std::wcout << *node->getIncludeNodeContent( ) << std::endl;
		}
	}
	std::wcout << L"↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑" << std::endl;
}
/// <summary>
/// 转化一个html 节点
/// </summary>
/// <param name="html_doc_shared">html 文档</param>
void htmlDocNodeConverToXDir( const cylHtmlTools::HtmlDoc_Shared &html_doc_shared ) {
	auto divNodes = html_doc_shared->findNodes( [&]( cylHtmlTools::HtmlNode_Shared &node_shared ) {
		if( cylHtmlTools::HtmlStringTools::equRemoveSpaceOverHtmlString( *node_shared->getNodeName( ), L"div" ) )
			return true;
		return false;
	} );
	std::wcout << L"=============== 测试把一个 htmldoc 转换到 Dir" << std::endl;
	for( auto &nodeSPtr : *divNodes ) {
		auto converLastXDir = nodeSPtr->converXDirSptr( );
		if( !converLastXDir ) {
			std::wcout << *nodeSPtr->getNodeName( ) << L" 转换失败";
			continue;
		}
		auto dirName = converLastXDir->getDirName( );
		auto xdirName = converLastXDir->getXDirName( );
		std::wcout << L"\t发现名称 : \"" << dirName << '\"' << std::endl;
		std::wcout << L"\t发现路径 : \"" << xdirName << '\"' << std::endl;
		auto maps = converLastXDir->getAttributeMaps( );
		auto iterator = maps.begin( );
		auto end = maps.end( );
		for( ; iterator != end; ++iterator ) {
			std::wcout << L"\t\t获得路径名称:\"" << iterator->first << '\"' << std::endl;
			for( auto &str : *iterator->second ) {
				auto &xdirAttribute = *str;
				auto name = xdirAttribute.getName( );
				if( name )
					std::wcout << L"\t\t\t获得属性名称:\"" << *name << '\"' << std::endl;
				auto htmlStringSPtrShared = xdirAttribute.getValues( );
				if( htmlStringSPtrShared )
					for( auto &value : *htmlStringSPtrShared )
						std::wcout << L"\t\t\t\t获得属性值:\"" << *value << '\"' << std::endl;
			}
			std::wcout << "-----------------------------------------------" << std::endl;
		}
	}
	std::wcout << L"===============" << std::endl;
}
/// <summary>
/// 测试：把一个 node 节点转换到 XDirAttribute 
/// </summary>
/// <param name="parem"></param>
void testHtmlNodeAttributeConverToXDirAttribute( const cylHtmlTools::HtmlString &parem ) {
	std::wcout << L"=============== 测试节点属性转换到 Vector_XDirAttributeSPtr_Shared" << std::endl;
	std::wcout << L"\t转换:\"" << parem << '\"' << std::endl;

	auto dirAttributes = cylHtmlTools::HtmlDoc::converNodeAttributeToXDirAttributes( parem.c_str( ), parem.length( ) );
	if( dirAttributes )
		for( auto &xdirAttribute : *dirAttributes ) {
			auto name = xdirAttribute->getName( );
			if( name )
				std::wcout << L"\t\t获得属性名称:\"" << *name << '\"' << std::endl;
			auto htmlStringSPtrShared = xdirAttribute->getValues( );
			if( htmlStringSPtrShared )
				for( auto &value : *htmlStringSPtrShared )
					std::wcout << L"\t\t\t获得属性值:\"" << *value << '\"' << std::endl;
		}
	else
		std::wcout << L"转换失败" << std::endl;
	std::wcout << L"===============" << std::endl;
}
int main( int argc, char *argv[ ] ) {
	std::locale locale( "zh_CN.UTF8" );
	std::locale::global( locale );
	std::wcout.imbue( locale );
	std::cout.imbue( locale );
	//testHtmlThreadNull( );
	//testHtmlThread( );
	//testHtmlThreadSharedNull( );
	//testHtmlThreadShared( );

	//int htmlDoc = testHtmlDoc( locale );
	//return htmlDoc;

	testXAttribute( LR"(@class="23 31" 123 " 3 11 ")" );
	testXAttribute( LR"(@acd="23 31" 123 " 3 11 ")" );
	testXAttributeIsIncludeOther( LR"(@acd="23 31" 123 " 3 11 ")", { LR"(23)" } );
	testXAttributeIsIncludeOther( LR"(@acd="23 31" 123 " 3 11 ")", { LR"(123)" } );
	testXAttributeIsOtherInclude( LR"(@acd="23 31" 123 " 3 11 ")",
		{ LR"(23 31)"
			, LR"(123)"
			, LR"( 3 11 )"
			, LR"(23)"
			, LR"(777)" }
	);
	testXAttributeIsOtherInclude( LR"(@acd="23 31" 123 " 3 11 ")",
		{ LR"(23 31)"
			, LR"( 3 11 )"
			, LR"(23)"
			, LR"(777)" }
	);
	testXDir( LR"(div[@"id"="sitebox sd" @class="cf ds"])" );
	testXPath( LR"(div[@"id"="sitebox" @class="cf"])" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox" class="cf")" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox" class="cf de")" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox bs" class="cf de")" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox bs" sitebox1 bs2 class="cf de")" );
	testHtmlNodeAttributeConverToXDirAttribute( LR"(id="sitebox bs" sitebox1 bs2 class="cf de" cf3 de4)" );
	testIncludeXDir( LR"(div[@"id"="sitebox sd" @class="cf ds"])", LR"(div[@"id"="sitebox sd" @class="cf ds"])" );
	testIncludeXDir( LR"(div[@"id"="sitebox sd" @class="cf ds es"])", LR"(div[@"id"="sitebox sd" @class="cf ds"])" );
	testIncludeXDir( LR"(div[@"id"="sitebox sd" @class="cf ds"])", LR"(div[@"id"="sitebox sd" @class="cf ds  es"])" );
	std::string fString( u8"%s/%s/%s" );
	char path[ 4096 ]{ 0 };
	int len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Project_Run_bin ).c_str( ), u8"writeFile", u8"wuxia.html" );
	if( len == 0 )
		return 1;
	auto wstringstream = getFileAllWString( path );
	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"wuxia.html" );
	if( len == 0 )
		return 2;
	size_t fileAllWString = setFileAllWString( path, wstringstream.str( ) );

	auto string = std::make_shared< cylHtmlTools::HtmlString >( wstringstream.str( ) );
	size_t endIndex = string->size( ), startIndex = 0;
	auto htmlDoc = cylHtmlTools::HtmlDoc::parse( string, endIndex, startIndex );
	if( !htmlDoc )
		return 3;
	htmlDocNodeConverToXDir( htmlDoc );
	findElements( htmlDoc, LR"(div[@"id"="sitebox"])" );
	findElements( htmlDoc, LR"(div[@"id"="sitebox"]/dl/dd/h3/a)" );
	findElements( htmlDoc, LR"(div[@"id"="sitebox" @class="cfe"]/dl/dd/h3/a)" );
	findElements( htmlDoc, LR"(div[@"id"="sitebox" @class="cf"]/dl/dd/h3/a)" );
	return 0;

}
