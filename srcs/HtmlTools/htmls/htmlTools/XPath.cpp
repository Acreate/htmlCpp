#include "XPath.h"

#include <iostream>


#include "XDir.h"
#include "../../wstr/HtmlStringTools.h"
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
				dirListSPtr.emplace_back( std::make_shared< XDir >( *string ) );
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

				dirListSPtr.emplace_back( std::make_shared< XDir >( *string ) );
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
		if( value == doubleQuotation ) {
			subStrVector.emplace_back( value );
			for( ++index; index < length; ++index ) {
				value = wstr[ index ];
				subStrVector.emplace_back( value );
				if( value == doubleQuotation )
					break;
			}
			continue;
		}
		if( value == charValue::singleQuotation ) {
			subStrVector.emplace_back( value );
			for( ++index; index < length; ++index ) {
				value = wstr[ index ];
				subStrVector.emplace_back( value );
				if( value == singleQuotation )
					break;
			}
			continue;
		}
		if( HtmlStringTools::isRouteChar( value ) ) {
			dataSize = subStrVector.size( );
			if( dataSize > 0 ) {
				auto data = subStrVector.data( );
				auto htmlStr = std::make_shared< HtmlString >( data, dataSize );
				dirListSPtr.emplace_back( std::make_shared< XDir >( *htmlStr ) );
				subStrVector.clear( );
			}
		} else
			subStrVector.emplace_back( value );
	}
	dataSize = subStrVector.size( );
	if( subStrVector.size( ) != 0 ) {
		auto data = subStrVector.data( );
		auto htmlStr = std::make_shared< HtmlString >( data, dataSize );

		dirListSPtr.emplace_back( std::make_shared< XDir >( *htmlStr ) );
	}
}
XPath::XPath( const List_HtmlStringSptr &std_w_string_list_shared, const HtmlString &separator )
	: separator( separator ) {
	for( auto& stdWString : std_w_string_list_shared )
		dirListSPtr.emplace_back( std::make_shared< XDir >( *stdWString ) );
}
XPath::~XPath( ) { }

static void findItem( Vector_HtmlNodeSPtr_Shared buff, XDir_Shared &subPath, Vector_HtmlNodeSPtr_Shared findNodes ) {
	auto nodeIterator = findNodes->begin( );
	auto nodeEnd = findNodes->end( );
	for( ; nodeIterator != nodeEnd; ++nodeIterator ) {
		if( nodeIterator == nodeEnd )
			break;
		auto nodeWsName = *nodeIterator->get( )->getNodeName( );
		if( subPath->hasName( nodeWsName ) ) {
			HtmlStringPairUnorderMap_Shared analysisAttribute = nodeIterator->get( )->analysisAttribute( );
			if( subPath->hasAttribute( analysisAttribute, nodeWsName ) )
				buff->emplace_back( *nodeIterator );
		}
	}
}
Vector_HtmlNodeSPtr_Shared XPath::rootBuider( HtmlDoc_Shared &html_doc ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	// 获取所有根节点
	auto analysisBrotherNode = html_doc->analysisBrotherNode( ); // 必须解析族谱

	HtmlDoc *element = html_doc.get( );
	auto findNodes( std::make_shared< Vector_HtmlNodeSPtr >( ) );
	for( auto& node : *element->getHtmlNodeRoots( ) )
		findNodes->emplace_back( node );
	auto buff( std::make_shared< Vector_HtmlNodeSPtr >( ) );
	size_t strListSize = dirListSPtr.size( ), index = 1;
	auto ptr = dirListSPtr.data( );
	do {
		auto &subPath = ptr[ index ];
		findItem( buff, subPath, findNodes );

		if( buff->size( ) == 0 )
			break;
		++index;// 判断下一个下标是否超出
		if( index < strListSize ) {  // 继续时，获取已经得到的子节点
			if( buff->size( ) == 0 ) // 找不到
				break;
			findNodes->clear( );
			for( auto& nodeShared : *buff ) {
				auto nodeChildrenNodes = nodeShared->subChildren;
				for( auto& children : *nodeChildrenNodes )
					findNodes->emplace_back( children );
			}
			buff->clear( );
			continue;
		}
		break;
	} while( true );
	if( index == strListSize )
		for( auto& node : *buff )
			result->emplace_back( node );
	return result;
}
Vector_HtmlNodeSPtr_Shared XPath::relativeBuider( HtmlNode &html_node_shared ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	// 获取所有根节点
	html_node_shared.analysisAttribute( ); // 必须解析族谱

	auto &findNodes = html_node_shared.subChildren;
	auto buff( std::make_shared< Vector_HtmlNodeSPtr >( ) );
	size_t strListSize = dirListSPtr.size( ), index = 1;
	auto ptr = dirListSPtr.data( );
	do {
		auto &subPath = ptr[ index ];
		findItem( buff, subPath, findNodes );

		if( buff->size( ) == 0 )
			break;
		++index;// 判断下一个下标是否超出
		if( index < strListSize ) {  // 继续时，获取已经得到的子节点
			if( buff->size( ) == 0 ) // 找不到
				break;
			findNodes->clear( );
			for( auto& nodeShared : *buff ) {
				auto nodeChildrenNodes = nodeShared->subChildren;
				for( auto& children : *nodeChildrenNodes )
					findNodes->emplace_back( children );
			}
			buff->clear( );
			continue;
		}
		break;
	} while( true );
	if( index == strListSize )
		for( auto& node : *buff )
			result->emplace_back( node );
	return result;
}
Vector_HtmlNodeSPtr_Shared XPath::anyBuider( Vector_HtmlNodeSPtr &html_node_shared ) {
	return { };
}
XPath::XPath( ) {

}
Vector_HtmlNodeSPtr_Shared XPath::buider( Vector_HtmlNodeSPtr_Shared &html_node_shared_s ) {
	if( dirListSPtr.size( ) == 0 )
		return nullptr;

	Vector_HtmlNodeSPtr_Shared resultShared( new Vector_HtmlNodeSPtr );
	auto &removeExtent = dirListSPtr[ 0 ];
	auto basicString = removeExtent->getDirName( );
	HtmlString first( 1, charValue::forwardSlash );
	if( basicString == first ) { // 存在 / 在最前面
		auto htmlNodeSharedS = *html_node_shared_s;
		for( auto& nodeShared : htmlNodeSharedS ) {
			auto sharedPtrs = rootBuider( nodeShared->htmldocShared );
			for( auto& node : *sharedPtrs )
				resultShared->emplace_back( node );
		}
		return resultShared;
	}
	first = HtmlString( 1, dot );
	if( basicString == first ) { // .
		auto htmlNodeSharedS = *html_node_shared_s;
		for( auto& nodeShared : htmlNodeSharedS ) {
			auto sharedPtrs = relativeBuider( *nodeShared );
			for( auto& node : *sharedPtrs )
				resultShared->emplace_back( node );
		}
		return resultShared;
	}

	return resultShared;
}
Vector_HtmlNodeSPtr_Shared XPath::buider( HtmlDoc_Shared &html_doc_shared ) {
	if( dirListSPtr.size( ) == 0 )
		return nullptr;
	Vector_HtmlNodeSPtr_Shared resultShared = rootBuider( html_doc_shared );
	if( resultShared->size( ) == 0 )
		return nullptr;
	return resultShared;
}
XPath::operator std::wstring( )
const {
	HtmlString result;
	auto iterator = dirListSPtr.begin( );
	auto end = dirListSPtr.end( );
	if( iterator != end ) {
		auto subStr = iterator->get( )->getDirName( );
		if( subStr.length( ) == 1 ) {
			if( HtmlStringTools::isRouteChar( subStr[ 0 ] ) ) {
				++iterator;
				result = result.append( subStr );
			}
		}
		do {
			subStr = iterator->get( )->getDirName( );
			++iterator;
			if( iterator == end ) {
				result = result.append( subStr );
				break;
			}
			result = result.append( subStr ) + charValue::forwardSlash;
		} while( true );
	}
	return result;
}
