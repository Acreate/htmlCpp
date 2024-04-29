#include "XPath.h"

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
	// 找到开始的 /
	for( ; index < length; ++index ) {
		HtmlChar value = wstr[ index ];
		if( HtmlStringTools::isJumpSpace( value ) )
			continue;
	}

	// 分解 / 路径
	for( ; index < length; ++index ) {
		HtmlChar value = wstr[ index ];
		if( value == forwardSlash ) {
			size_t dataSize = subStrVector.size( );
			if( dataSize > 0 ) {
				auto data = subStrVector.data( );
				auto htmlStr = std::make_shared< HtmlString >( data, dataSize );
				stdWStringListShared.emplace_back( htmlStr );
				subStrVector.clear( );
			}
		} else
			subStrVector.emplace_back( value );
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
	auto findNodes = html_doc->getHtmlNodeRoots( );
	auto buff( std::make_shared< Vector_HtmlNodeSPtr >( ) );
	size_t strListSize = stdWStringListShared.size( ), index = 0;
	auto ptr = stdWStringListShared.data( );
	for( ; index < strListSize; ++index ) {
		auto &subPath = ptr[ index ];
		auto nodeIterator = findNodes->begin( );
		auto nodeEnd = findNodes->end( );
		for( ; nodeIterator != nodeEnd; ++nodeIterator ) {
			if( nodeIterator == nodeEnd )
				break;
			if( subPath == nodeIterator->get( )->getNodeWSName( ) )
				buff->emplace_back( *nodeIterator );
		}
		if( buff->size( ) == 0 )
			break;
		*findNodes = *buff;
		buff->clear( );
	}
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
