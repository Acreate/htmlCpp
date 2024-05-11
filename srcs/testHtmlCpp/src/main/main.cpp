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
void testHtmlThread( ) {

	int coun = 22;
	cylHtmlTools::HtmlWorkThread< int * >::Start_Thread_Run< int * > startThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"开始一个线程 :(发现数据) : " << *data << std::endl;
	};

	// 测试回调
	startThreadRun( nullptr, nullptr, nullptr, nullptr, &coun );

	cylHtmlTools::HtmlWorkThread< int * >::Current_Thread_Run< int * > currentThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data, const time_t *startTime ) {
		std::this_thread::sleep_for( std::chrono::seconds( 3 ) );
		std::cout << u8"线程正在工作 :(发现数据) : " << *data << std::endl;
		*data = 777;
		std::cout << u8"线程正在工作 :(改变数据) : " << *data << std::endl;
	};
	// 测试回调
	currentThreadRun( nullptr, nullptr, nullptr, nullptr, &coun, nullptr );

	cylHtmlTools::HtmlWorkThread< int * >::Finish_Thread_Run< int * > finishThreadRun = [=]( const cylHtmlTools::HtmlWorkThread< int * > *html_work_thread, const std::thread *run_std_cpp_thread, std::mutex *html_work_thread_mutex, std::mutex *std_cpp_thread_mutex, int *data ) {
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
int main( int argc, char *argv[ ] ) {
	std::locale locale( "zh_CN.UTF8" );
	std::locale::global( locale );
	std::wcout.imbue( locale );
	std::cout.imbue( locale );
	testHtmlThread( );
	return 0;
	return testHtmlDoc( locale );
}
