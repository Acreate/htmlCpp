#include "HtmlNode.h"

#include "../htmlDoc/HtmlDoc.h"
#include "../htmlTools/XPath/XPath.h"
#include "../../htmlString/HtmlStringTools.h"
#include "../htmlDoc/Tools/HtmlDocTools.h"
#include <sstream>
#include <iostream>

using namespace cylHtmlTools;
using namespace cylHtmlTools::charValue;

HtmlNode::HtmlNode( ) : parent( nullptr ), subChildren( new Vector_HtmlNodeSPtr ), brother( new Vector_HtmlNodeSPtr ) {
}
HtmlNode::~HtmlNode( ) {
}

std::vector< size_t > HtmlNode::converStringToHtmlNodeAttributeEqu( const wchar_t *str_w_char_data_ptr, const size_t count ) {

	std::vector< size_t > result;
	for( size_t index = 0; index < count; ++index )
		if( str_w_char_data_ptr[ index ] == singleQuotation ) {
			for( ; index < count; ++index )
				if( str_w_char_data_ptr[ index ] == singleQuotation )
					break;
		} else if( str_w_char_data_ptr[ index ] == doubleQuotation ) {
			for( ; index < count; ++index )
				if( str_w_char_data_ptr[ index ] == doubleQuotation )
					break;
		} else if( str_w_char_data_ptr[ index ] == equ )
			result.emplace_back( index );
	return result;
}

std::vector< std::pair< HtmlString, HtmlString > > HtmlNode::converStringToHtmlNodeAttributePair( const wchar_t *str_w_char_data_ptr, const size_t &count ) {
	std::vector< std::pair< HtmlString, HtmlString > > result;
	size_t index = 0;
	HtmlChar currentChar = str_w_char_data_ptr[ index ];
	// 缩小范围
	for( ; index < count; ++index )
		if( HtmlStringTools::isSpace( str_w_char_data_ptr[ index ] ) == false )
			break;
	size_t buffMaxCount = count - 1;
	for( ; index < buffMaxCount; --buffMaxCount )
		if( str_w_char_data_ptr[ buffMaxCount ] == nodeEndChar )
			break;

	// 属性名称
	std::vector< HtmlString::value_type > key;
	// 属性值
	std::vector< HtmlString::value_type > value;
	// 操作指针
	std::vector< HtmlString::value_type > *buff = &key;
	// 当前操作属性类型，0 表示关键字，1 表示值，2 表示存储
	size_t currentType = 0;
	// 当前引号，0 表示不存在，1 表示单引号，2 表示双引号
	size_t currentQuotationType = 0;
	// 拷贝字符串的起始地址
	while( index < buffMaxCount ) {
		currentChar = str_w_char_data_ptr[ index ];
		if( currentChar == singleQuotation ) {
			if( currentQuotationType == 0 )
				currentQuotationType = 1; // 激活单引号标识
			else if( currentQuotationType == 1 ) {
				currentQuotationType = 0; // 已经匹配单引号标识
				if( currentType == 0 ) {
					currentType = 1; // 切换到值
					buff = &value;
				} else if( currentType == 1 )
					currentType = 2; // 切换到存储
			}
		} else if( currentChar == doubleQuotation ) {
			if( currentQuotationType == 0 )
				currentQuotationType = 2; // 激活双引号标识
			else if( currentQuotationType == 2 ) {
				currentQuotationType = 0; // 已经匹配双单引号标识
				if( currentType == 0 ) {
					currentType = 1; // 切换到值
					buff = &value;
				} else if( currentType == 1 )
					currentType = 2; // 切换到存储
			}
		} else if( currentChar == equ ) {
			value.clear( );
			buff = &value;
			currentType = 1; // 切换到值
			++index;
			continue;
		}
		if( currentType == 2 ) {
			buff->emplace_back( currentChar );
			HtmlString keyHtmlString( key.data( ), key.size( ) );
			HtmlString valueHtmlString( value.data( ), value.size( ) );
			result.emplace_back( keyHtmlString, valueHtmlString );
			key.clear( );
			value.clear( );
			buff = &key;
			currentType = 0; // 切换到关键字
			for( ++index; index < buffMaxCount; ++index )
				if( HtmlStringTools::isSpace( str_w_char_data_ptr[ index ] ) == false )
					break;
		}
		buff->emplace_back( str_w_char_data_ptr[ index ] );
		++index;
	}
	if( currentType != 0 )
		result.emplace_back( HtmlString( key.data( ), key.size( ) ), HtmlString( value.data( ), value.size( ) ) );
	return result;
}

UMap_HtmlStringK_HtmlStringV_Shared HtmlNode::analysisAttribute( ) {
	if( refNodeAttributes ) {
		if( nodeType == Html_Node_Type::DoubleNode && endNode.get( ) == this ) {
			endNode->refNodeAttributes = startNode->refNodeAttributes;
			return startNode->analysisAttribute( );
		}
		return refNodeAttributes;
	}
	refNodeAttributes = std::make_shared< UMap_HtmlStringK_HtmlString >( );

	auto startWStrPtr = this->czWStr->c_str( ) + ptrOffset;
	size_t equIndex = 0, endIndex = ptrCWtrLen;
	wchar_t currentChar = L'\0';
	for( ; equIndex < endIndex; ++equIndex ) {
		currentChar = startWStrPtr[ equIndex ];
		if( currentChar == nodeStartChar ) { // 找到名称
			++equIndex;
			currentChar = startWStrPtr[ equIndex ];
			if( !HtmlStringTools::isSpace( currentChar ) ) { // 不是空，则是名称
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					if( HtmlStringTools::isSpace( currentChar ) )
						break;
				}
			} else { // 如果是空，则找到一个非空
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					if( !HtmlStringTools::isSpace( currentChar ) ) {
						++equIndex;
						for( ; equIndex < endIndex; ++equIndex ) { // 找到末尾的空，或者结束该节点
							currentChar = startWStrPtr[ equIndex ];
							if( HtmlStringTools::isSpace( currentChar ) )
								break;
						}
						break;
					}
				}
			}
			break;
		}

	}
	// todo : 解析属性列表
	auto node = htmldocShared->getNodeContent( thisSharedPtr );
	size_t count = endIndex - equIndex;
	if( count == 0 )
		return refNodeAttributes;
	auto attributeString = converStringToHtmlNodeAttributePair( startWStrPtr + equIndex, count );
	count = attributeString.size( );
	auto data = attributeString.data( );
	for( equIndex = 0; equIndex < count; ++equIndex )
		refNodeAttributes->insert_or_assign( data[ equIndex ].first, data[ equIndex ].second );
	return refNodeAttributes;
}
HtmlString_Shared HtmlNode::getNodeContent( ) const {
	return htmldocShared->getNodeContent( thisSharedPtr );
}
HtmlString_Shared HtmlNode::getNodeName( ) const {
	if( isEndNode( ) )
		return htmldocShared->getNodeName( startNode );
	return htmldocShared->getNodeName( thisSharedPtr );
}
Html_Node_Type HtmlNode::getNodeType( ) const {
	return thisSharedPtr->nodeType;
}
HtmlNode_Shared HtmlNode::getStartNode( ) const {
	return startNode;
}
HtmlNode_Shared HtmlNode::getEndNode( ) const {
	return endNode;
}
size_t HtmlNode::nodeSize( ) const {
	if( nodeType == Html_Node_Type::DoubleNode )
		return endNode->ptrOffset + endNode->ptrCWtrLen - startNode->ptrOffset;
	return ptrCWtrLen;
}
size_t HtmlNode::getPtrOffset( ) const {
	return ptrOffset;
}
size_t HtmlNode::getPtrCWtrLen( ) const {
	return ptrCWtrLen;
}
HtmlString_Shared HtmlNode::getIncludeNodeContent( ) const {
	if( isEndNode( ) )
		return htmldocShared->getIncludeNodeContent( startNode );
	return htmldocShared->getIncludeNodeContent( thisSharedPtr );
}
HtmlString_Shared HtmlNode::getPath( ) const {
	return htmldocShared->getPath( thisSharedPtr );
}
HtmlString_Shared HtmlNode::getNodeIncludeContentText( ) const {
	return htmldocShared->getNodeContentText( thisSharedPtr );
}
UMap_HtmlStringK_HtmlStringV_Shared HtmlNode::findAttribute( const std::function< bool( const HtmlString &attributeName, const HtmlString &attributeValue ) > &callFunction ) const {
	return htmldocShared->findAttribute( thisSharedPtr, callFunction );
}
Vector_HtmlNodeSPtr_Shared HtmlNode::xpath( const HtmlString &xpath ) {
	XPath xPath( xpath );
	auto htmlNodeSharedS = std::make_shared< Vector_HtmlNodeSPtr >( );
	htmlNodeSharedS->emplace_back( this->thisSharedPtr );
	return xPath.buider( htmlNodeSharedS );
}
Vector_HtmlNodeSPtr_Shared HtmlNode::getHtmlNodeRoots( ) {
	return htmldocShared->getHtmlNodeRoots( );
}

bool HtmlNode::isStartNode( ) const {
	return htmldocShared->isStartNode( *this );
}
bool HtmlNode::isEndNode( ) const {
	return htmldocShared->isEndNode( *this );
}
bool HtmlNode::isDoubleNodeType( ) const {
	return htmldocShared->isDoubleNodeType( *this );
}
Vector_HtmlNodeSPtr_Shared HtmlNode::parseHtmlNodeCharPair( const HtmlDoc_Shared &html_doc_shared, size_t start_index, const size_t max_index, size_t &index_count ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	bool findCharResut = false;
	index_count = start_index;
	auto stdCWString = html_doc_shared->htmlWCStr;
	for( ; start_index < max_index; ++start_index ) {
		auto maxIndex = max_index;
		auto startIndex = start_index;
		findCharResut = HtmlDocTools::findNextNodeStartChar( stdCWString, maxIndex, startIndex );
		if( !findCharResut )
			break;
		start_index = startIndex;
		auto ptr = new HtmlNode;
		HtmlNode_Shared currentHtmlNodeCharPairSharedPtr( ptr );
		ptr->ptrOffset = start_index;
		startIndex = start_index;
		findCharResut = HtmlDocTools::findNextNodeEndChar( stdCWString, maxIndex, startIndex );
		if( !findCharResut )
			break;
		start_index = startIndex;
		ptr->ptrCWtrLen = start_index + 1 - ptr->ptrOffset;
		ptr->czWStr = stdCWString;
		ptr->htmldocShared = html_doc_shared;
		ptr->thisSharedPtr = currentHtmlNodeCharPairSharedPtr;
		result->emplace_back( currentHtmlNodeCharPairSharedPtr );
	}
	index_count = start_index - index_count;
	return result;
}

void HtmlNode::setParent( const HtmlNode_Shared &child, const HtmlNode_Shared &parent ) {
	if( child == nullptr || child.get( ) == parent.get( ) )
		return;
	auto parentPtr = child->parent.get( );
	if( parentPtr ) {
		auto vectorHtmlXPathSPtrShared = parentPtr->subChildren;
		auto iterator = vectorHtmlXPathSPtrShared->begin( ), end = vectorHtmlXPathSPtrShared->end( );
		for( ; iterator != end; ++iterator ) {
			auto removeExtent = iterator->get( );
			if( removeExtent == child.get( ) ) {
				vectorHtmlXPathSPtrShared.get( )->erase( iterator );
				break;
			}
		}
	}
	if( parent )
		parent->subChildren->emplace_back( child );
	child->parent = parent;
}
