#include "XPath.h"

#include "../htmlNode/HtmlNode.h"
#include "../htmlDoc/HtmlDoc.h"
using namespace htmlTools;
using namespace htmlTools::charValue;
XPath::XPath( const std::wstring &wstr ) : XPath( ) {
	stdWStringListShared.emplace_back( std::make_shared< std::wstring >( wstr ) );
}
XPath::XPath( const List_HtmlStringSptr &std_w_string_list_shared, const HtmlString &separator )
	: separator( separator ) {
	for( auto &stdWString : std_w_string_list_shared )
		stdWStringListShared.emplace_back( std::make_shared< HtmlString >( *stdWString ) );
}
XPath::~XPath( ) { }
XPath::XPath( ) {

}
Vector_HtmlNodeSPtr_Shared XPath::buider( HtmlNode_Shared html_node_shared ) {
	if( stdWStringListShared.size( ) == 0 )
		return nullptr;
	Vector_HtmlNodeSPtr_Shared resultShared( new Vector_HtmlNodeSPtr );
	Vector_HtmlNodeSPtr findNodes;
	HtmlString htmlString = getHtmlString( );
	auto pathStart = htmlString[ 0 ]; // 路径开始字符

	/// 路径转换
	size_t htmlStringSize = htmlString.size( ); // 字符串大小
	using Type = decltype(pathStart); // 定义缓冲类型
	Type *dataPtr = new Type[ htmlStringSize ]; // 缓冲指针
	for( size_t index = 0; index < htmlStringSize; ++index ) {
		Type charInStr = htmlString[ index ];
		if( charInStr == backSlash )
			dataPtr[ index ] = forwardSlash;
		else
			dataPtr[ index ] = charInStr;
	}
	htmlString = HtmlString( dataPtr, htmlStringSize );
	delete[] dataPtr;
	auto pathSecond = htmlString[ 1 ];
	if( pathStart == forwardSlash && pathSecond == pathStart ) { // 返回顶级路径
		// 出现 // 开头
		auto htmlDoc = html_node_shared.get( )->htmldocShared;
		auto brotherNode = htmlDoc->analysisBrotherNode( );
		for( auto &node : *brotherNode )
			if( !node->parent ) // 存入顶级节点
				findNodes.emplace_back( node );
	} else if( pathStart == dot && pathSecond == forwardSlash ) { // 
		// 出现 ./ 开头
		findNodes.emplace_back( html_node_shared );
	}
	/// 开始查找

	return nullptr;
}
