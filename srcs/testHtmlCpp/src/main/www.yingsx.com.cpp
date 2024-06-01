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

// u8R"(div[@class='cf' @id='sitebox' ]/dl)" )
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
	std::string fString( u8"%s/%s" );
	auto fileName = u8"www.121ds.cc_9_index.html";
	char path[ 4096 ] = { 0 };
	int len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Project_Restources_Dir ).c_str( ), fileName );
	std::cout << u8"==================" << std::endl;
	std::cout << u8"读取文件 : " << path << std::endl;
	std::cout << u8"==================" << std::endl;
	if( len == 0 )
		return 1;
	auto wstringstream = cylHtmlTools::HtmlDocTools::getFileAllWString( path );
	if( !wstringstream )
		return -1;
	len = snprintf( path, sizeof( path ), fString.c_str( ), std::string( Cache_Path_Dir ).c_str( ), fileName );
	if( len == 0 )
		return 2;
	size_t fileAllWString = cylHtmlTools::HtmlDocTools::setFileAllWString( path, *wstringstream );
	auto htmlDoc = cylHtmlTools::HtmlDoc::parse( wstringstream );
	if( !htmlDoc )
		return 3;
	wstringstream->clear( );
	size_t index = 0;
	auto nodes = htmlDoc->xpathRootNodes( LR"(div[@class='cf' @id="sitebox" ]/dl)" );
	if( nodes ) {
		for( auto node : *nodes ) {
			
		}

	}

	return 0;
}
