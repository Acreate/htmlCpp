#include "HtmlDoc.h"

#include <algorithm>

#include "../../macro/cmake_to_c_cpp_header_env.h"
#include "../../htmlString/HtmlStringTools.h"
#include "../HtmlNode/HtmlNode.h"
#include "../htmlTools/XPath/XPath.h"
#include "../enum/HtmlNodeType/Html_Node_Type.h"

#include <clocale>
#include <iostream>
#include <memory>

#include <sstream>
#include <string>

#include "../htmlTools/XDir/XDir.h"
#include "../htmlTools/XDirAttribute/XDirAttribute.h"
#include "Tools/HtmlDocTools.h"
using namespace cylHtmlTools;
using namespace cylHtmlTools::charValue;


HtmlDoc_Shared HtmlDoc::parse( const HtmlString_Shared &std_c_w_string ) {
	HtmlDoc_Shared result( new HtmlDoc );
	result->thisStdShared = result;
	result->htmlWCStr = std_c_w_string;
	auto stdCWString = result->htmlWCStr;
	size_t count;
	auto resultHtml = HtmlNode::parseHtmlNodeCharPair( result, 0, std_c_w_string->length( ), count );

	auto htmlNodeCharPairs = resultHtml.get( );
	size_t maxSize = htmlNodeCharPairs->size( );
	size_t index = 0;
	for( ; index < maxSize; ++index ) {
		auto &htmlDocCharPair = htmlNodeCharPairs->at( index );
		auto hasPtr = false;
		HtmlNode *htmlNode = htmlDocCharPair.get( );
		for( auto &ptr : *result->htmlDocNode ) {
			if( htmlNode == ptr.get( ) ) {
				hasPtr = true;
				break;
			}
			if( htmlDocCharPair->nodeType == Html_Node_Type::DoubleNode ) {
				if( htmlDocCharPair->endNode.get( ) == ptr.get( ) ) {
					hasPtr = true;
					break;
				}
			}
		}
		if( hasPtr )
			continue;
		auto left = htmlNode->ptrOffset;
		auto right = htmlNode->ptrCWtrLen + left;
		bool nodeType = HtmlDocTools::isAnnotation( stdCWString, left, right );
		if( nodeType && left < right ) {
			htmlDocCharPair->nodeType = Html_Node_Type::AnnotationNode;
			result->htmlDocNode->emplace_back( htmlDocCharPair );
		} else {
			left = htmlNode->ptrOffset;
			right = htmlNode->ptrCWtrLen + left;
			nodeType = HtmlDocTools::isSingelNode( stdCWString, left, right );
			if( nodeType ) {
				htmlDocCharPair->nodeType = Html_Node_Type::SingleNode;
				result->htmlDocNode->emplace_back( htmlDocCharPair );
			} else {
				left = htmlNode->ptrOffset;
				size_t endLeft = left;
				right = htmlNode->ptrCWtrLen + endLeft;
				if( HtmlDocTools::isStartNode( stdCWString, endLeft, right ) ) {
					size_t lastNodeIndex = index + 1;
					size_t endNodeIndex = maxSize;
					auto vectorHtmlXPathSPtrShared = HtmlDocTools::analysisDoubleNode( result, htmlDocCharPair, resultHtml, lastNodeIndex, endNodeIndex );
					auto htmlNodeIterator = vectorHtmlXPathSPtrShared->begin( );
					auto endNode = vectorHtmlXPathSPtrShared->end( );
					for( ; htmlNodeIterator != endNode; ++htmlNodeIterator )
						result->htmlDocNode->emplace_back( *htmlNodeIterator );

				}
			}
		}
	}
	return result;
}
HtmlNode_Shared HtmlDoc::findNodeFromName( const HtmlString &nodeName ) const {
	for( auto &node : *htmlDocNode.get( ) )
		if( *getNodeName( node ) == nodeName )
			return node;
	return nullptr;
}
HtmlNode_Shared HtmlDoc::findNodeFromName( const std::function< bool( const HtmlString &nodeName, Html_Node_Type htmlNodeType ) > &callFun ) const {
	for( auto &node : *htmlDocNode.get( ) )
		if( callFun( *getNodeName( node ), node->nodeType ) )
			return node;

	return nullptr;
}
Vector_HtmlNodeSPtr_Shared HtmlDoc::findNodes( const std::function< bool( HtmlNode_Shared & ) > &callFun ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	for( auto &node : *htmlDocNode.get( ) )
		if( callFun( node ) )
			result->emplace_back( node );

	if( result->size( ) == 0 )
		return nullptr;
	return result;
}
HtmlDoc::HtmlDoc( ) : htmlDocNode( new Vector_HtmlNodeSPtr ) {

}
HtmlDoc::~HtmlDoc( ) {
}

Vector_HtmlNodeSPtr_Shared HtmlDoc::analysisBrotherNode( ) {
	if( analysisOver )
		return analysisOver;
	analysisOver = std::make_shared< Vector_HtmlNodeSPtr >( );// 有父节点
	Vector_HtmlNodeSPtr_Shared noneParent( new Vector_HtmlNodeSPtr ); // 无父节点
	auto htmlNodes = htmlDocNode.get( );
	bool isOverAnalysis = false;
	for( auto &htmlNodeSPtr : *htmlNodes ) {
		HtmlNode *element = htmlNodeSPtr.get( );
		if( element->nodeType == Html_Node_Type::DoubleNode && element == element->endNode.get( ) )
			continue;
		// 校验父节点解析
		for( auto &overAnalysis : *analysisOver )
			if( *overAnalysis.get( ) == *element ) { // 如果已经解析完毕，则跳过节点
				isOverAnalysis = true;
				break;
			}
		if( isOverAnalysis ) { // 跳过已经解析完毕的节点
			isOverAnalysis = false;
			continue;
		}

		auto parent = htmlNodeSPtr->parent;
		if( parent ) { // 存在父节点才存在兄弟节点
			analysisOver->emplace_back( htmlNodeSPtr ); // 保存到父节点列表当中
			// 遍历父节点引用的子节点，并把该节点引用到兄弟节点当中（跳过自身节点）
			for( auto &parentSubChildren : *parent->subChildren )
				if( *parentSubChildren.get( ) != *element )
					htmlNodeSPtr->brother->emplace_back( parentSubChildren );
			continue;
		}
		noneParent->emplace_back( htmlNodeSPtr ); // 保存到无根节点列表当中
	}
	if( noneParent->size( ) == 0 )
		return analysisOver;
	auto cloneNoneParent = *noneParent;
	// 遍历无根节点
	for( auto &htmlNodeSPtr : *noneParent ) {

		HtmlNode *element = htmlNodeSPtr.get( );
		if( element->nodeType == Html_Node_Type::DoubleNode && element == element->endNode.get( ) )
			continue;
		// 校验父节点解析
		for( auto &overAnalysis : *analysisOver )
			if( *overAnalysis.get( ) == *element ) {
				isOverAnalysis = true;
				break;
			}
		if( isOverAnalysis ) {
			isOverAnalysis = false;
			continue;
		}
		analysisOver->emplace_back( htmlNodeSPtr );
		// 遍历友邻节点
		for( auto &brotherNode : cloneNoneParent )
			if( *element != *brotherNode.get( ) )
				htmlNodeSPtr->brother->emplace_back( brotherNode );
	}
	return analysisOver;
}
Vector_HtmlNodeSPtr_Shared HtmlDoc::analysisAttributesNode( ) {
	Vector_HtmlNodeSPtr_Shared analysisOver( new Vector_HtmlNodeSPtr );

	auto &vector = *htmlDocNode.get( );
	for( auto &nodePtr : vector ) {
		auto &node = *nodePtr;
		node.analysisAttribute( );
		analysisOver->emplace_back( nodePtr );
	}
	return analysisOver;
}
bool HtmlDoc::isStartNode( const HtmlNode_Shared &node_shared ) const {
	return isDoubleNodeType( node_shared ) && node_shared.get( ) == node_shared->startNode.get( );
}
bool HtmlDoc::isEndNode( const HtmlNode_Shared &node_shared ) const {
	return isDoubleNodeType( node_shared ) && node_shared.get( ) == node_shared->endNode.get( );
}
bool HtmlDoc::isDoubleNodeType( const HtmlNode_Shared &node_shared ) const {
	return node_shared->nodeType == Html_Node_Type::DoubleNode && node_shared->startNode && node_shared->endNode;
}
bool HtmlDoc::isStartNode( const HtmlNode &node ) const {
	return isDoubleNodeType( node ) && &node == node.startNode.get( );
}
bool HtmlDoc::isEndNode( const HtmlNode &node ) const {
	return isDoubleNodeType( node ) && &node == node.endNode.get( );
}
bool HtmlDoc::isDoubleNodeType( const HtmlNode &node ) const {
	return node.nodeType == Html_Node_Type::DoubleNode && node.startNode.get( ) && node.endNode.get( );
}
HtmlString_Shared HtmlDoc::getNodeContent( const HtmlNode_Shared &node_shared ) const {
	if( isEndNode( node_shared ) && node_shared->startNode )
		return getNodeContent( node_shared->startNode );
	auto c_w_str_star_ptr = node_shared->czWStr->c_str( ) + node_shared->ptrOffset;
	HtmlString_Shared result( new HtmlString( c_w_str_star_ptr, node_shared->ptrCWtrLen ) );
	return result;
}
HtmlString_Shared HtmlDoc::getNodeName( const HtmlNode_Shared &node_shared ) const {
	wchar_t currentChar = zero; // 临时字符
	auto c_w_str = node_shared->czWStr->c_str( ) + node_shared->ptrOffset; // 字符串指向坐标
	size_t index = 0;
	for( ; index < node_shared->ptrCWtrLen; ++index ) { // 找到 < 后面的非空
		currentChar = c_w_str[ index ];
		if( currentChar == nodeStartChar || HtmlStringTools::isSpace( currentChar ) )
			continue;
		break;
	}
	c_w_str = c_w_str + index; // 指向第一个非空字符
	auto ptrCStrLen = node_shared->ptrCWtrLen - index; // 缩减长度
	for( index = 0; index < ptrCStrLen; ++index ) { // 找到第一个空或者 / > 等字符
		currentChar = c_w_str[ index ];
		if( HtmlStringTools::isSpace( currentChar ) || currentChar == nodeEndChar || currentChar == forwardSlash )
			break;
	}
	if( currentChar == forwardSlash ) { // 如果碰到斜杠 /(节点是尾节点)
		for( ; index < ptrCStrLen; ++index ) { // 找到第一个非空或者 > 等字符
			currentChar = c_w_str[ index ];
			if( !HtmlStringTools::isSpace( currentChar ) ) {
				++index;
				c_w_str = c_w_str + index;
				for( index = 0; index < ptrCStrLen; ++index ) { // 找到第一个空或者 > 等字符
					currentChar = c_w_str[ index ];
					if( HtmlStringTools::isSpace( currentChar ) || currentChar == nodeEndChar )
						break;
				}
				break;
			}
		}
	}
	HtmlString_Shared result( new HtmlString( c_w_str, index ) );
	return result;
}


Html_Node_Type HtmlDoc::getNodeType( const HtmlNode_Shared &node_shared ) const {
	return node_shared->nodeType;
}
std::shared_ptr< HtmlNode > HtmlDoc::getStartNode( const HtmlNode_Shared &node_shared ) const { return node_shared->startNode; }
std::shared_ptr< HtmlNode > HtmlDoc::getEndNode( const HtmlNode_Shared &node_shared ) const { return node_shared->endNode; }
size_t HtmlDoc::nodeSize( const HtmlNode_Shared &node_shared ) const {
	if( isDoubleNodeType( node_shared ) && node_shared->endNode && node_shared->startNode )
		return node_shared->endNode->ptrOffset + node_shared->endNode->ptrCWtrLen - node_shared->startNode->ptrOffset;
	return node_shared->ptrCWtrLen;
}
size_t HtmlDoc::getPtrOffset( const HtmlNode_Shared &node_shared ) const { return node_shared->ptrOffset; }
size_t HtmlDoc::getPtrCWtrLen( const HtmlNode_Shared &node_shared ) const { return node_shared->ptrCWtrLen; }
HtmlString_Shared HtmlDoc::getIncludeNodeContent( const HtmlNode_Shared &node_shared ) const {
	if( isEndNode( node_shared ) && node_shared->startNode )
		return getIncludeNodeContent( node_shared->startNode );
	return std::make_shared< HtmlString >( node_shared->czWStr->c_str( ) + node_shared->ptrOffset, nodeSize( node_shared ) );
}
HtmlString_Shared HtmlDoc::getPath( const HtmlNode_Shared &node_shared ) const {
	auto nodeName = *getNodeName( node_shared );
	HtmlString_Shared result( new HtmlString( L"/" + nodeName ) );

	HtmlNode_Shared parent = node_shared->parent;
	while( parent ) {
		nodeName = *getNodeName( parent );
		*result = L"/" + nodeName + *result;
		parent = parent->parent;
	}

	return result;
}
HtmlString_Shared HtmlDoc::getNodeContentText( const HtmlNode_Shared &node_shared ) const {
	if( node_shared->nodeType == Html_Node_Type::AnnotationNode || node_shared->nodeType == Html_Node_Type::SingleNode ) {
		HtmlString_Shared htmlStringShared = this->getNodeName( node_shared );
		if( HtmlStringTools::equRemoveSpaceOverHtmlString( *htmlStringShared, L"br" ) )
			return std::make_shared< HtmlString >( L"\n" );
		return nullptr;
	}
	size_t startIndex, endIndex;
	auto startNode = node_shared->startNode;
	startIndex = startNode->ptrOffset + startNode->ptrCWtrLen;
	endIndex = node_shared->getEndNode( )->ptrOffset - startIndex;
	if( endIndex == 0 )
		return nullptr;
	return HtmlDocTools::htmlStringContentTextConverToHtmlString( htmlWCStr->c_str( ) + startIndex, endIndex );

}
XDir_Shared HtmlDoc::converXDirSptr( const HtmlNode_Shared &node_shared ) {
	if( node_shared->nodeType == Html_Node_Type::AnnotationNode )
		return nullptr; // todo  注释未实现
	if( isEndNode( node_shared ) )
		return converXDirSptr( node_shared->startNode );
	auto startHtmlCharPtr = htmlWCStr->c_str( ) + node_shared->ptrOffset + 1; // 跳过 < 符号
	auto newLen = node_shared->ptrCWtrLen - 2; // 不包含 > 符号
	size_t nameIndex = 0;
	// 过滤空格
	for( ; nameIndex < newLen; ++nameIndex )
		if( !HtmlStringTools::isSpace( startHtmlCharPtr[ nameIndex ] ) )
			break;
	size_t nameLen = nameIndex;
	// 查找名称
	for( ; nameLen < newLen; ++nameLen )
		if( HtmlStringTools::isSpace( startHtmlCharPtr[ nameLen ] ) )
			break;
	nameLen -= nameIndex;
	HtmlString xdirName( startHtmlCharPtr + nameIndex, nameLen ); // 找到名称
	nameIndex += nameLen;
	// 过滤空格
	for( ; nameIndex < newLen; ++nameIndex )
		if( !HtmlStringTools::isSpace( startHtmlCharPtr[ nameIndex ] ) )
			break;
	Vector_XDirAttributeSPtr_Shared xDirAttributes = converNodeAttributeToXDirAttributes( startHtmlCharPtr + nameIndex, newLen - nameIndex );
	return std::make_shared< XDir >( xdirName, xDirAttributes );
}
Vector_XDirAttributeSPtr_Shared HtmlDoc::converNodeAttributeToXDirAttributes(
	const HtmlChar *conver_buff, const size_t conver_buff_len ) {
	Vector_XDirAttributeSPtr_Shared result( std::make_shared< Vector_XDirAttributeSPtr >( ) );
	size_t index = 0;
	size_t buffIndex = 0;
	do {
		if( HtmlStringTools::findNextHtmlCharPotion( conver_buff, conver_buff_len, charValue::equ, index ) ) { // 找到
			auto attributeName = XDirAttribute::converXDirAttributeName( conver_buff + buffIndex, index );
			++index;
			buffIndex = index;
			if( HtmlStringTools::findNextHtmlCharPotion( conver_buff, conver_buff_len, charValue::equ, buffIndex ) ) { // 再次查找
				--buffIndex;
				// 检测 = 前的空格
				for( ; buffIndex > index; --buffIndex )
					if( !HtmlStringTools::isSpace( conver_buff[ buffIndex ] ) )
						break;
				--buffIndex;
				// 找到空格
				for( ; buffIndex > index; --buffIndex )
					if( HtmlStringTools::isSpace( conver_buff[ buffIndex ] ) )
						break;
				auto attributeValue = XDirAttribute::converXDirAttributeValues( conver_buff + index, buffIndex - index, attributeName );
				result->emplace_back( std::make_shared< XDirAttribute >( attributeName, attributeValue ) );
				index = buffIndex + 1;
			} else { // 没有等号，则往后的全是值
				auto attributeValue = XDirAttribute::converXDirAttributeValues( conver_buff + index, conver_buff_len - index, attributeName );
				result->emplace_back( std::make_shared< XDirAttribute >( attributeName, attributeValue ) );
				break;
			}

			if( index > conver_buff_len )
				break;
		} else {
			auto attributeName = XDirAttribute::converXDirAttributeName( conver_buff + index, conver_buff_len - index );
			result->emplace_back( std::make_shared< XDirAttribute >( attributeName ) );
			break;
		}
	} while( true );
	if( result->size( ) == 0 )
		return nullptr;
	return result;
}

Vector_HtmlNodeSPtr_Shared HtmlDoc::matchChildrenNodes( const HtmlNode_Shared &node_shared, const std::function< bool( const HtmlNode_Shared & ) > callFunction ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );

	for( auto &node : *node_shared->subChildren )
		if( callFunction( node ) )
			result->emplace_back( node );

	return result;
}
Vector_HtmlNodeSPtr_Shared HtmlDoc::xpathRootNodes( const XPath &xpath ) {
	return xpath.buider( getHtmlNodeRoots( ) );
}
Vector_HtmlNodeSPtr_Shared HtmlDoc::xpathRootNodes( const HtmlString &xpath ) {
	return XPath( xpath ).buider( getHtmlNodeRoots( ) );
}
Vector_HtmlNodeSPtr_Shared HtmlDoc::xpathAllNode( const HtmlString &xpath ) {
	XPath xPath( xpath );
	return xPath.buider( this->htmlDocNode );
}
Vector_HtmlNodeSPtr_Shared HtmlDoc::getHtmlNodeRoots( ) {
	if( htmlNodeSPtrRoots )
		return htmlNodeSPtrRoots;
	htmlNodeSPtrRoots = std::make_shared< Vector_HtmlNodeSPtr >( );
	auto docNode = *analysisAttributesNode( );
	for( auto &htmlNodeSPtr : docNode )
		if( htmlNodeSPtr->parent == nullptr )
			htmlNodeSPtrRoots->emplace_back( htmlNodeSPtr );
	return htmlNodeSPtrRoots;
}
UMap_HtmlStringK_HtmlStringV_Shared HtmlDoc::findAttribute( const HtmlNode_Shared &node_shared, const std::function< bool( const HtmlString &, const HtmlString & ) > &callFunction ) const {
	UMap_HtmlStringK_HtmlStringV_Shared result( std::make_shared< UMap_HtmlStringK_HtmlString >( ) );
	auto iterator = node_shared->refNodeAttributes->begin( );
	auto end = node_shared->refNodeAttributes->end( );
	for( ; iterator != end; ++iterator )
		if( callFunction( iterator->first, iterator->second ) )
			result->emplace( iterator->first, iterator->second );
	if( result->size( ) == 0 )
		return nullptr;
	return result;
}
