#include "XPath.h"

#include <iostream>


#include "../../wstr/WStrTools.h"
#include "../htmlNode/HtmlNode.h"
#include "../htmlDoc/HtmlDoc.h"
using namespace htmlTools;
using namespace htmlTools::charValue;
XPath::XPath( const HtmlString &wstr ) : XPath( ) {
	size_t length = wstr.length( );
	if( length == 0 || wstr.empty( ) )
		return;
	std::vector< size_t > forwardSlashIndes;
	std::vector< HtmlChar > subStrVector;
	size_t index = 0;
	HtmlChar value;
	// 处理开端的 / (/ 和 //)
	for( ; index < length; ++index ) {
		value = wstr[ index ];
		if( HtmlStringTools::isJumpSpace( value ) )
			continue;
		if( HtmlStringTools::isRouteChar( value ) ) { // 找到第一个 /
			++index;
			value = wstr[ index ];
			if( HtmlStringTools::isRouteChar( value ) ) {
				auto string = std::make_shared< HtmlString >( );
				*string += forwardSlash;
				stdWStringListShared.emplace_back( string );
				++index;
				do {
					value = wstr[ index ];
					if( value != forwardSlash || !HtmlStringTools::isJumpSpace( value ) )
						break;
					++index;
				} while( true );
			}
			break;
		}
		if( value == dot ) { // 处理 ./ 开头
			++index;
			value = wstr[ index ];
			if( HtmlStringTools::isRouteChar( value ) ) {
				auto string = std::make_shared< HtmlString >( );
				*string += dot;
				stdWStringListShared.emplace_back( string );
				++index;
				do {
					value = wstr[ index ];
					if( HtmlStringTools::isRouteChar( value ) || !HtmlStringTools::isJumpSpace( value ) )
						break;
					++index;
				} while( true );
				break;
			}
			throw L"路径不支持 ../";
		}
		break;
	}


	size_t dataSize;
	// 分解 / 路径
	for( ; index < length; ++index ) {
		value = wstr[ index ];
		if( HtmlStringTools::isRouteChar( value ) ) {
			dataSize = subStrVector.size( );
			if( dataSize > 0 ) {
				auto data = subStrVector.data( );
				auto htmlStr = std::make_shared< HtmlString >( data, dataSize );
				stdWStringListShared.emplace_back( htmlStr );
				subStrVector.clear( );
			}
		} else
			subStrVector.emplace_back( value );
	}
	dataSize = subStrVector.size( );
	if( subStrVector.size( ) != 0 ) {
		auto data = subStrVector.data( );
		auto htmlStr = std::make_shared< HtmlString >( data, dataSize );
		stdWStringListShared.emplace_back( htmlStr );
	}
}
XPath::XPath( const List_HtmlStringSptr &std_w_string_list_shared, const HtmlString &separator )
	: separator( separator ) {
	for( auto &stdWString : std_w_string_list_shared )
		stdWStringListShared.emplace_back( std::make_shared< HtmlString >( *stdWString ) );
}
XPath::~XPath( ) { }
Vector_HtmlNodeSPtr_Shared XPath::rootBuider( HtmlDoc_Shared html_doc ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	// 获取所有根节点
	auto analysisBrotherNode = html_doc->analysisBrotherNode( ); // 必须解析族谱
	for( auto &node : *analysisBrotherNode )
		std::wcout << *node->getPath( ) << std::endl;
	HtmlDoc *element = html_doc.get( );
	auto findNodes = element->getHtmlNodeRoots( );
	auto buff( std::make_shared< Vector_HtmlNodeSPtr >( ) );
	size_t strListSize = stdWStringListShared.size( ), index = 1;
	auto ptr = stdWStringListShared.data( );
	do {
		auto nodeIterator = findNodes->begin( );
		auto nodeEnd = findNodes->end( );
		auto &subPath = *ptr[ index ];
		for( ; nodeIterator != nodeEnd; ++nodeIterator ) {
			if( nodeIterator == nodeEnd )
				break;
			auto nodeWsName = *nodeIterator->get( )->getNodeName( );
			if( subPath == nodeWsName )
				buff->emplace_back( *nodeIterator );
		}
		if( buff->size( ) == 0 )
			break;
		++index;// 判断下一个下标是否超出
		if( index < strListSize ) {  // 继续时，获取已经得到的子节点
			if( buff->size( ) == 0 ) // 找不到
				break;
			findNodes->clear( );
			for( auto &nodeShared : *buff ) {
				auto nodeChildrenNodes = nodeShared->subChildren;
				for( auto &children : *nodeChildrenNodes )
					findNodes->emplace_back( children );
			}
			buff->clear( );
			continue;
		}
		break;
	} while( true );
	return result;
}
Vector_HtmlNodeSPtr_Shared XPath::relativeBuider( Vector_HtmlNodeSPtr html_node_shared ) {
	return { };
}
Vector_HtmlNodeSPtr_Shared XPath::anyBuider( Vector_HtmlNodeSPtr html_node_shared ) {
	return { };
}
XPath::XPath( ) {

}
Vector_HtmlNodeSPtr_Shared XPath::buider( Vector_HtmlNodeSPtr html_node_shared_s ) {
	if( stdWStringListShared.size( ) == 0 )
		return nullptr;
	Vector_HtmlNodeSPtr_Shared resultShared( new Vector_HtmlNodeSPtr );
	Vector_HtmlNodeSPtr findNodes;
	HtmlString htmlString = getHtmlString( );
	auto pathStart = htmlString[ 0 ]; // 路径开始字符
	auto pathSecond = htmlString[ 1 ];
	if( pathStart == forwardSlash ) {
		if( pathSecond == pathStart ) { // 出现 // 开头

		} else { // 顶部只有一个 /
		}

	}
	if( pathStart == dot && pathSecond == forwardSlash ) { // 

	}
	if( resultShared->size( ) == 0 )
		return nullptr;
	return resultShared;
}
Vector_HtmlNodeSPtr_Shared XPath::buider( HtmlDoc_Shared html_doc_shared ) {
	if( stdWStringListShared.size( ) == 0 )
		return nullptr;
	Vector_HtmlNodeSPtr_Shared resultShared = rootBuider( html_doc_shared );
	if( resultShared->size( ) == 0 )
		return nullptr;
	return resultShared;
}
