#include "HtmlDoc.h"

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

using namespace cylHtmlTools;
using namespace cylHtmlTools::charValue;


bool HtmlDoc::findNextNodeEndChar( const HtmlString_Shared &std_c_w_string, size_t &max_index, size_t &start_index ) {
	auto CWStrLen = std_c_w_string->length( );
	if( max_index > CWStrLen )
		max_index = CWStrLen;
	auto CWStr = std_c_w_string->c_str( );
	for( ; start_index < max_index; ++start_index ) {
		auto currenChar = CWStr[ start_index ];
		if( currenChar == singleQuotation ) {
			++start_index;
			for( ; start_index < max_index; ++start_index ) {
				currenChar = CWStr[ start_index ];
				if( currenChar == singleQuotation )
					break;
			}
		} else if( currenChar == doubleQuotation ) {
			++start_index;
			for( ; start_index < max_index; ++start_index ) {
				currenChar = CWStr[ start_index ];
				if( currenChar == doubleQuotation )
					break;
			}
		} else if( currenChar == nodeEndChar )
			return true;
	}
	return false;
}
bool HtmlDoc::findNextNodeStartChar( const HtmlString_Shared &std_c_w_string, size_t &max_index, size_t &start_index ) {
	auto cwStrPtr = std_c_w_string->c_str( );
	auto cwStrLen = std_c_w_string->length( );
	if( max_index > cwStrLen )
		max_index = cwStrLen;
	for( ; start_index < max_index; ++start_index ) {
		auto currenChar = cwStrPtr[ start_index ];
		if( currenChar == singleQuotation ) {
			++start_index;
			for( ; start_index < max_index; ++start_index ) {
				currenChar = cwStrPtr[ start_index ];
				if( currenChar == singleQuotation )
					break;
			}
		} else if( currenChar == doubleQuotation ) {
			++start_index;
			for( ; start_index < max_index; ++start_index ) {
				currenChar = cwStrPtr[ start_index ];
				if( currenChar == doubleQuotation )
					break;
			}
		} else if( currenChar == exclamation ) {
			++start_index;
			for( ; start_index < max_index; ++start_index ) {
				currenChar = cwStrPtr[ start_index ];
				if( currenChar == nodeStartChar )
					break;
			}
		} else if( currenChar == nodeStartChar )
			return true;
	}
	return false;
}
bool HtmlDoc::findNextNodeForwardSlash( const HtmlString_Shared &std_c_w_string, size_t &max_index, size_t &start_index ) {
	auto cwzStrPtr = std_c_w_string.get( )->c_str( );
	auto cwzStrLen = std_c_w_string->length( );
	if( max_index > cwzStrLen )
		max_index = cwzStrLen;
	for( ; start_index < max_index; ++start_index ) {
		auto currenChar = cwzStrPtr[ start_index ];
		if( currenChar == singleQuotation ) {
			++start_index;
			for( ; start_index < max_index; ++start_index ) {
				currenChar = cwzStrPtr[ start_index ];
				if( currenChar == singleQuotation )
					break;
			}
		} else if( currenChar == doubleQuotation ) {
			++start_index;
			for( ; start_index < max_index; ++start_index ) {
				currenChar = cwzStrPtr[ start_index ];
				if( currenChar == doubleQuotation )
					break;
			}
		} else if( currenChar == exclamation ) {
			++start_index;
			for( ; start_index < max_index; ++start_index ) {
				currenChar = cwzStrPtr[ start_index ];
				if( currenChar == nodeEndChar )
					break;
			}
		} else if( currenChar == forwardSlash )
			return true;
	}
	return false;
}
bool HtmlDoc::isSingelNode( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index ) {
	auto c_str = std_c_w_string->c_str( );
	auto currentChar = c_str[ start_index ];
	if( currentChar != nodeStartChar )
		for( ++start_index; start_index < end_index; ++start_index ) {
			currentChar = c_str[ start_index ];
			if( currentChar == nodeStartChar )
				break;
		}
	auto forwardSlashIndex = start_index + 1;
	if( findNextNodeForwardSlash( std_c_w_string, end_index, forwardSlashIndex ) ) {
		for( forwardSlashIndex += 1; forwardSlashIndex < end_index; ++forwardSlashIndex ) {
			currentChar = c_str[ forwardSlashIndex ];
			if( HtmlStringTools::isSpace( currentChar ) )
				continue;
			if( currentChar != nodeEndChar )
				break;
			end_index = forwardSlashIndex;
			return true;
		}
	}
	return false;
}
bool HtmlDoc::isStartNode( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index ) {
	auto c_str = std_c_w_string->c_str( );
	auto currentChar = c_str[ start_index ];
	if( currentChar != nodeStartChar )
		for( ++start_index; start_index < end_index; ++start_index ) {
			currentChar = c_str[ start_index ];
			if( currentChar == nodeStartChar )
				break;
		}

	// 碰到的第一个必须是 > 而不是 /
	for( auto index = start_index + 1; index <= end_index; ++index ) {
		currentChar = c_str[ index ];
		if( HtmlStringTools::isSpace( currentChar ) )
			continue;
		if( currentChar == doubleQuotation ) {
			++index;
			for( ; index <= end_index; ++index ) {
				currentChar = c_str[ index ];
				if( currentChar == doubleQuotation )
					break;
			}
		} else if( currentChar == singleQuotation ) {
			++index;
			for( ; index <= end_index; ++index ) {
				currentChar = c_str[ index ];
				if( currentChar == singleQuotation )
					break;
			}
		} else if( currentChar == forwardSlash )
			return false;
		else {
			return true;
		}
	}
	return false;
}
bool HtmlDoc::isEndNode( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index ) {
	auto c_str = std_c_w_string->c_str( );
	wchar_t currentChar = 0;
	// 碰到的第一个必须是 / 而不是通用字符或者 >
	for( ; start_index <= end_index; ++start_index ) {
		currentChar = c_str[ start_index ];
		if( currentChar == nodeStartChar )
			break;
	}
	for( auto index = start_index + 1; index <= end_index; ++index ) {
		currentChar = c_str[ index ];
		if( HtmlStringTools::isSpace( currentChar ) )
			continue;
		if( currentChar != forwardSlash )
			return true;
	}
	return false;
}
bool HtmlDoc::isAnnotation( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index ) {
	auto c_str = std_c_w_string->c_str( );
	auto currentChar = c_str[ start_index ];
	if( currentChar != nodeStartChar )
		for( ++start_index; start_index < end_index; ++start_index ) {
			currentChar = c_str[ start_index ];
			if( currentChar == nodeStartChar )
				break;
		}
	size_t endIndex = start_index + 1;

	for( ; endIndex < end_index; ++endIndex ) {
		currentChar = c_str[ endIndex ];
		if( currentChar == exclamation ) {
			if( findNextNodeEndChar( std_c_w_string, end_index, endIndex ) ) {
				end_index = endIndex;
				return true;
			}
			start_index = 1;
			end_index = 0;
			return false;
		}
	}
	return false;
}

Vector_HtmlNodeSPtr_Shared HtmlDoc::analysisDoubleNode( const HtmlDoc_Shared &html_doc_shared, HtmlNode_Shared html_start_node, Vector_HtmlNodeSPtr_Shared &html_node_char_pairs, size_t &start_index, size_t &end_index ) {

	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	std::list< HtmlNode_Shared > htmlNodeSharedTack;
	htmlNodeSharedTack.emplace_back( html_start_node );
	result->emplace_back( html_start_node );
	size_t left, right, endLeft;
	auto stdCWString = html_doc_shared->htmlWCStr;
	bool nodeType;
	for( ; start_index < end_index; ++start_index ) {
		auto &htmlNode = html_node_char_pairs->at( start_index );
		left = htmlNode.get( )->ptrOffset;
		right = htmlNode.get( )->ptrCWtrLen + left;
		nodeType = isAnnotation( stdCWString, left, right );
		if( nodeType ) {
			// 跳过注释节点
			htmlNode->nodeType = Html_Node_Type::AnnotationNode;
			if( htmlNodeSharedTack.size( ) > 0 )
				htmlNode->setParent( htmlNodeSharedTack.back( ) );
			result->emplace_back( htmlNode );
			continue;
		}
		left = htmlNode.get( )->ptrOffset;
		right = htmlNode.get( )->ptrCWtrLen + left;
		nodeType = isSingelNode( stdCWString, left, right );
		if( nodeType ) {
			// 跳过单节点
			htmlNode->nodeType = Html_Node_Type::SingleNode;
			if( htmlNodeSharedTack.size( ) > 0 )
				htmlNode->setParent( htmlNodeSharedTack.back( ) );
			result->emplace_back( htmlNode );
			continue;
		}
		left = htmlNode.get( )->ptrOffset;
		endLeft = left;
		right = htmlNode.get( )->ptrCWtrLen + left;
		nodeType = isStartNode( stdCWString, left, right );
		if( nodeType ) {
			htmlNode->nodeType = Html_Node_Type::DoubleNode;
			htmlNode->startNode = htmlNode;
			if( htmlNodeSharedTack.size( ) > 0 )
				htmlNode->setParent( htmlNodeSharedTack.back( ) );
			htmlNodeSharedTack.emplace_back( htmlNode );
			result->emplace_back( htmlNode );
			continue;
		}
		nodeType = isEndNode( stdCWString, endLeft, right );
		if( !nodeType ) // 不是结束节点则跳过
			continue;
		auto endNodeName = *html_doc_shared->getNodeName( htmlNode );
		// 找到同名节点
		size_t index = htmlNodeSharedTack.size( );
		for( ; index > 0; --index ) {
			auto nodeSharedTack = htmlNodeSharedTack.back( );
			auto tackNode = *html_doc_shared->getNodeName( nodeSharedTack );
			if( HtmlStringTools::equRemoveSpaceOverHtmlString( tackNode, endNodeName ) )
				break;
			nodeSharedTack->nodeType = Html_Node_Type::SingleNode;
			htmlNodeSharedTack.pop_back( );
			// 跳过开头节点
			if( htmlNodeSharedTack.size( ) > 0 )
				nodeSharedTack->setParent( htmlNodeSharedTack.back( ) );
			else
				nodeSharedTack->setParent( nullptr );
		}
		if( htmlNodeSharedTack.size( ) == 0 )
			break;
		auto startNode = htmlNodeSharedTack.back( );

		startNode->nodeType = Html_Node_Type::DoubleNode;
		htmlNode->nodeType = Html_Node_Type::DoubleNode;

		htmlNode->endNode = htmlNode;
		startNode->endNode = htmlNode;

		htmlNode->startNode = startNode;
		startNode->startNode = startNode;

		htmlNodeSharedTack.pop_back( );
		if( htmlNodeSharedTack.size( ) > 0 ) {
			auto topParent = htmlNodeSharedTack.back( );
			auto topParentPrr = topParent.get( );
			if( topParentPrr && htmlNode.get( ) != topParentPrr && htmlNode->endNode.get( ) != topParentPrr && htmlNode->startNode.get( ) != topParentPrr )
				htmlNode->setParent( topParent );
		} else
			break;
	}
	return result;
}

HtmlDoc_Shared HtmlDoc::parse( const HtmlString_Shared &std_c_w_string, size_t &end_index, size_t &start_index ) {
	HtmlDoc_Shared result( new HtmlDoc );
	result->thisStdShared = result;
	result->htmlWCStr = std::make_shared< HtmlString >( std_c_w_string->c_str( ) + start_index, end_index - start_index );
	auto stdCWString = result->htmlWCStr;
	size_t count;
	auto resultHtml = HtmlNode::parseHtmlNodeCharPair( result, 0, end_index, count );

	auto htmlNodeCharPairs = resultHtml.get( );
	size_t maxSize = htmlNodeCharPairs->size( );
	size_t index = start_index;
	start_index = 0;
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
		bool nodeType = isAnnotation( stdCWString, left, right );
		if( nodeType && left < right ) {
			htmlDocCharPair->nodeType = Html_Node_Type::AnnotationNode;
			result->htmlDocNode->emplace_back( htmlDocCharPair );
		} else {
			left = htmlNode->ptrOffset;
			right = htmlNode->ptrCWtrLen + left;
			nodeType = isSingelNode( stdCWString, left, right );
			if( nodeType ) {
				htmlDocCharPair->nodeType = Html_Node_Type::SingleNode;
				result->htmlDocNode->emplace_back( htmlDocCharPair );
			} else {
				left = htmlNode->ptrOffset;
				size_t endLeft = left;
				right = htmlNode->ptrCWtrLen + endLeft;
				if( isStartNode( stdCWString, endLeft, right ) ) {
					size_t lastNodeIndex = index + 1;
					size_t endNodeIndex = maxSize;
					auto vectorHtmlXPathSPtrShared = analysisDoubleNode( result, htmlDocCharPair, resultHtml, lastNodeIndex, endNodeIndex );
					auto htmlNodeIterator = vectorHtmlXPathSPtrShared->begin( );
					auto endNode = vectorHtmlXPathSPtrShared->end( );
					for( ; htmlNodeIterator != endNode; ++htmlNodeIterator )
						result->htmlDocNode->emplace_back( *htmlNodeIterator );

				}
			}
		}
		if( index == 0 )
			start_index = htmlDocCharPair.get( )->ptrOffset;
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
	if( node_shared->nodeType == Html_Node_Type::AnnotationNode || node_shared->nodeType == Html_Node_Type::SingleNode )
		return getNodeContent( node_shared );
	auto startNode = node_shared->startNode;
	auto endNode = node_shared->endNode;
	auto offset = startNode->ptrOffset + startNode->ptrCWtrLen; // 开始节点的结束位置
	auto wstrPtr = htmlWCStr->c_str( ); // 字符串指针
	if( node_shared->subChildren->size( ) == 0 ) {
		HtmlString_Shared result( new HtmlString( wstrPtr + offset, endNode->ptrOffset - offset ) );
		return result;
	}
	std::wstringstream stringstream;
	auto iterator = startNode->subChildren->begin( ); // 迭代器
	auto end = startNode->subChildren->end( ); // 迭代器终止位置
	for( ; iterator != end; ++iterator ) {
		auto &nodeShared = *iterator;
		auto forPathName = getPath( nodeShared );
		if( nodeShared->ptrOffset > offset ) { // 检查开始位置
			// <a> A... <b> </b> </a>
			// 截取 A../
			auto wcstrStartPtr = wstrPtr + offset;
			HtmlString subWString( wcstrStartPtr, nodeShared->ptrOffset - offset );
			stringstream << subWString;
		}
		if( nodeShared->nodeType == Html_Node_Type::DoubleNode ) {
			// 循环节点当中的结束位置	
			size_t forPtrNodeEndIndex = nodeShared->endNode->ptrOffset + nodeShared->endNode->ptrCWtrLen;
			if( forPtrNodeEndIndex > offset )
				offset = forPtrNodeEndIndex + 1;
		} else {
			size_t forPtrNodeEndIndex = nodeShared->ptrOffset + nodeShared->ptrCWtrLen;
			if( forPtrNodeEndIndex > offset )
				offset = forPtrNodeEndIndex + 1;
		}
	}
	if( endNode->ptrOffset > offset ) {
		// <a> <b> </b> A... </a>
		// 截取 A../
		HtmlString subWString( wstrPtr + offset, endNode->ptrOffset - offset );
		stringstream << subWString;
	}
	auto mystr = stringstream.str( );
	HtmlString_Shared result( new HtmlString( mystr ) );
	return result;
}

Vector_HtmlNodeSPtr_Shared HtmlDoc::matchChildrenNodes( const HtmlNode_Shared &node_shared, const std::function< bool( const HtmlNode_Shared & ) > callFunction ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );

	for( auto &node : *node_shared->subChildren )
		if( callFunction( node ) )
			result->emplace_back( node );

	return result;
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
