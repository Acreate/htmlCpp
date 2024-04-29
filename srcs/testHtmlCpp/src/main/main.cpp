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
	std::cout << htmlTools::HtmlStringTools::wstringConverString( *htmlContent ) << std::endl;

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
	auto htmlDoc = htmlTools::HtmlDoc::parse( htmlContent, endIndex, startIndex );
	htmlDoc->findNodes( [&]( auto node ) ->bool {
		auto wsNode = *htmlDoc->getNodeContent( node );
		if( htmlDoc->getNodeType( node ) == htmlTools::Html_Node_Type::DoubleNode && htmlDoc->getEndNode( node ).get( ) != node.get( ) ) {
			auto wsNodeText = *htmlDoc->getIncludeNodeContent( node );
			std::wstringstream ss;
			ss << L"================" << std::endl;
			ss << wsNodeText << std::endl;
			ss << L"----------------" << std::endl;
			ss << *htmlDoc->getPath( node ) << std::endl;
			ss << L"================" << std::endl;

			std::string converString = htmlTools::HtmlStringTools::wstringConverString( ss.str( ) );
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
	// 解析族谱关系（父子，兄弟关系）
	htmlDoc->analysisBrotherNode( );
	// 解析所有节点关系 (单独解析可以使用 WStringPairUnorderMap_Shared HtmlNode::analysisAttribute( ))
	htmlDoc->analysisAttributesNode( );

	auto xpath = htmlTools::XPath( L"//html/body/div/div/div/div" );
	auto vectorHtmlNodeSPtrShared = xpath.buider( htmlDoc );
	if( !vectorHtmlNodeSPtrShared )
		return -1;
	for( auto &node : *vectorHtmlNodeSPtrShared )
		std::wcout << *node->getNodeName( ) << std::endl;
	return 0;
}
