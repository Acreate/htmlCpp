#include <codecvt>

#include "../macro/cmake_to_c_cpp_header_env.h"
#include <fstream>
#include <htmls/HtmlNode/HtmlNode.h>
#include <htmls/htmlDoc/HtmlDoc.h>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

#include "htmls/htmlTools/XPath.h"
#include "wstr/WStrTools.h"


int main( int argc, char *argv[ ] ) {
	std::locale locale( "zh_CN.UTF-8" );

	auto oldLocale = std::wcout.imbue( locale );

	std::string fString( u8"%s/%s/%s" );
	char path[ 4096 ]{ 0 };
	int len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Project_Run_bin ).c_str( ), u8"writeFile", u8"www.121ds.cc.html" );
	if( len == 0 )
		return -1;
	std::wifstream ifstream( path, std::ios::binary | std::ios::in );
	if( !ifstream.is_open( ) )
		return -2;
	ifstream.imbue( locale );
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
	ifstream.close( );
	// 写入文件
	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"www.121ds.cc.txt" );
	if( len == 0 )
		return -1;
	std::wofstream ofstream( path, std::ios::binary | std::ios::out | std::ios::trunc );
	ofstream.imbue( locale );
	if( !ofstream.is_open( ) )
		return -2;
	ofstream << stringstream.str( );
	ofstream.close( );

	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Project_Run_bin ).c_str( ), u8"writeFile", u8"www.121ds.cc.html" );
	ifstream.open( path, std::ios::binary | std::ios::in );
	if( !ifstream.is_open( ) )
		return -2;
	ifstream.imbue( locale );
	std::vector< wchar_t > buff;
	wchar_t *wBuff = new wchar_t[ 1024 ]{ 0 };
	do {
		auto &result = ifstream.get( wBuff, 1024, EOF );
		std::streamsize gcount = result.gcount( );
		size_t size = buff.size( );
		size_t newSize = size + gcount;
		buff.resize( newSize, 0 );
		auto data = buff.data( );
		size_t index = 0;
		wchar_t c;
		for( ; index < gcount; ++index ) {
			c = wBuff[ index ];
			if( c == EOF || c == 0 )
				break;
			data[ index + size ] = c;
		}
		if( gcount < 1024 - 1 )
			break;
	} while( true );
	ifstream.close( );

	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"www.121ds.cc.2.txt" );
	ofstream.open( path, std::ios::binary | std::ios::out | std::ios::trunc );
	if( !ofstream.is_open( ) )
		return -2;
	ofstream.imbue( locale );
	ofstream.write( buff.data( ), buff.size( ) );
	ofstream.close( );
	// 测试调用
	size_t endIndex = htmlContent->size( ), startIndex = 0;
	auto htmlDoc = htmlTools::HtmlDoc::parse( htmlContent, endIndex, startIndex );


	// 解析族谱关系（父子，兄弟关系）
	htmlDoc->analysisBrotherNode( );
	// 解析所有节点关系 (单独解析可以使用 HtmlStringPairUnorderMap_Shared HtmlNode::analysisAttribute( ))
	htmlDoc->analysisAttributesNode( );

	auto xpath = htmlTools::XPath( L"//html/body/div/div/div/div[@class='hd']/ul/li/a" );
	std::wcout << L"===============" << std::endl;
	std::wcout << xpath.getHtmlString( ) << std::endl;
	std::wcout << L"===============" << std::endl;
	auto htmlNodeRoots = htmlDoc->getHtmlNodeRoots( );
	auto vectorHtmlNodeSPtrShared = xpath.buider( htmlNodeRoots );
	if( !vectorHtmlNodeSPtrShared )
		return -1;
	for( auto &node : *vectorHtmlNodeSPtrShared ) {
		auto name = *node->getNodeName( );
		auto contentText = *node->getNodeContentText( );
		auto content = *node->getNodeContent( );
		auto path = *node->getPath( );
		std::wcout << L"=======================" << std::endl;
		std::wcout << L"找到 [" << name << L"]:" << path << std::endl;
		std::wcout << L"<<<<<<<<<<" L"\t" L"nodeText" L"\t" L"<<<<<<<<<<" << std::endl;
		std::wcout << contentText << std::endl;
		std::wcout << L"<<<<<<<<<<" L"\t" L"nodeContent" L"\t" L"<<<<<<<<<<" << std::endl;
		std::wcout << content << std::endl;
		std::wcout << L"=======================" << std::endl;

	}
	return 0;
}
