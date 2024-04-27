#include <codecvt>

#include "../macro/cmake_to_c_cpp_header_env.h"
#include <fstream>
#include <htmls/HtmlNode/HtmlNode.h>
#include <htmls/htmlDoc/HtmlDoc.h>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

#include "htmls/htmlTools/XPathTools.h"
#include "wstr/WStrTools.h"


int main( int argc, char *argv[ ] ) {


	auto xPathTools = HtmlTools::XPathTools( L"1234443" );
	std::wcout << xPathTools << std::endl;
	xPathTools = xPathTools - L"3";
	std::wcout << xPathTools << std::endl;
	std::string fString( u8"%s/%s/%s" );
	char path[ 4096 ]{ 0 };
	int len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Project_Run_bin ).c_str( ), u8"writeFile", u8"www.121ds.cc.html" );
	if( len == 0 )
		return -1;
	std::wifstream ifstream( path, std::ios::binary | std::ios::in );
	if( !ifstream.is_open( ) )
		return -2;

	std::wstringstream stringstream;
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
	std::cout << HtmlTools::WStrTools::wstringConverString( *htmlContent ) << std::endl;

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
	auto htmlDoc = HtmlTools::HtmlDoc::parse( htmlContent, endIndex, startIndex );
	htmlDoc->getNodes( [&]( auto node ) ->bool {
		auto wsNode = *htmlDoc->getWSNode( node );
		if( htmlDoc->getNodeType( node ) == HtmlTools::Html_Node_Type::DoubleNode && htmlDoc->getEndNode( node ).get( ) != node.get( ) ) {
			auto wsNodeText = *htmlDoc->getNodeText( node );
			std::wstringstream ss;
			ss << L"================" << std::endl;
			ss << wsNodeText << std::endl;
			ss << L"----------------" << std::endl;
			ss << *htmlDoc->getPath( node ) << std::endl;
			ss << L"================" << std::endl;

			std::string converString = HtmlTools::WStrTools::wstringConverString( ss.str( ) );
			std::cout << converString << std::endl;
			std::cout.flush( );
			std::cout.clear( );
			len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"www.121ds.cc.txt.node.write.txt" );
			if( len == 0 )
				return false;
			std::wofstream ofstream( path, std::ios::binary | std::ios::out | std::ios::trunc );
			if( !ofstream.is_open( ) )
				return false;
			ofstream << ss.str( );
			ofstream.close( );
			return false;
		}
		return false;
	} );
	return 0;
}
