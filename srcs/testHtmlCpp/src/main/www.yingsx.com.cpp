#include "www.yingsx.com.h"
#include <htmlString/HtmlStringTools.h>
#include <htmls/htmlDoc/HtmlDoc.h>
#include <htmls/htmlDoc/Tools/HtmlDocTools.h>
#include <htmls/htmlNode/HtmlNode.h>
#include <htmls/htmlTools/XDir/XDir.h>
#include <htmls/htmlTools/XDirAttribute/XDirAttribute.h>
#include <iostream>
#include <string>
#include "../auto_generate_files/macro/cmake_to_c_cpp_header_env.h"


void testXDir( cylHtmlTools::XDir xdir ) {
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

int test_www_yingsx_com( std::locale locak ) {
	std::string fString( u8"%s/%s/%s" );
	char path[ 4096 ]{ 0 };
	int len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Project_Run_bin ).c_str( ), u8"writeFile", u8"www.yingsx.com.html" );
	if( len == 0 )
		return 1;
	auto wstringstream = cylHtmlTools::HtmlDocTools::getFileAllWString( path );
	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"www.yingsx.com.html" );
	if( len == 0 )
		return 2;
	size_t fileAllWString = cylHtmlTools::HtmlDocTools::setFileAllWString( path, *wstringstream );

	*wstringstream = LR"(<input name="page" type="text" size="4" maxlength="6" onkeydown="if(event.keyCode==13){window.location='https://www.yingsx.com/xuanhuan/1_<{$page}>.html'.replace('<{$page|subdirectory}>', '/' + Math.floor(this.value / 1000)).replace('<{$page}>', this.value); return false;}" />)";
	auto htmlDoc = cylHtmlTools::HtmlDoc::parse( wstringstream );
	if( !htmlDoc )
		return 3;
	wstringstream->clear( );
	size_t index = 0;
	htmlDoc->findNodes( [&]( cylHtmlTools::HtmlNode_Shared &html_node_shared ) {
		cylHtmlTools::HtmlString buff( L"\n========================\t" );
		buff.append( std::to_wstring( ++index ) );
		buff.append( L"\n" );
		buff.append( *html_node_shared->getIncludeNodeContent( ) );
		buff.append( L"\n========================\n" );
		std::wcout << buff << std::endl;
		auto dir = html_node_shared->converXDirSptr( );
		if( dir )
			testXDir( *dir );
		wstringstream->append( buff );
		return false;
	} );
	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), u8"", u8"www.yingsx.com.txt" );
	if( len == 0 )
		return 4;
	fileAllWString = cylHtmlTools::HtmlDocTools::setFileAllWString( path, *wstringstream, std::ios::out | std::ios::trunc );
	return 0;
}
