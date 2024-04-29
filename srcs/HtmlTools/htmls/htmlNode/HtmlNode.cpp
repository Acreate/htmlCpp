#include "HtmlNode.h"


#include "../htmlDoc/HtmlDoc.h"
#include "../htmlTools/XPath.h"
#include "../../wstr/WStrTools.h"
#include <sstream>

using namespace htmlTools;
using namespace htmlTools::charValue;

HtmlNode::HtmlNode( ) : parent( nullptr ), subChildren( new Vector_HtmlNodeSPtr ), brother( new Vector_HtmlNodeSPtr ), refNodeAttributes( new WStringPairUnorderMap ) {
}
HtmlNode::~HtmlNode( ) {
}

WStringPairUnorderMap_Shared HtmlNode::analysisAttribute( ) {
	if( nodeType == Html_Node_Type::DoubleNode && endNode.get( ) == this ) {
		endNode->refNodeAttributes = startNode->refNodeAttributes;
		return startNode->analysisAttribute( );
	}
	if( refNodeAttributes->size( ) > 0 )
		return refNodeAttributes;
	WStringPairUnorderMap_Shared result( new WStringPairUnorderMap );

	auto startWStrPtr = this->czWStr->c_str( ) + ptrOffset;
	size_t equIndex = 0, endIndex = ptrCWtrLen;
	wchar_t currentChar = L'\0';
	for( ; equIndex < endIndex; ++equIndex ) {
		currentChar = startWStrPtr[ equIndex ];
		if( currentChar == nodeStartChar ) { // 找到名称
			++equIndex;
			currentChar = startWStrPtr[ equIndex ];
			if( !HtmlStringTools::isJumpSpace( currentChar ) ) { // 不是空，则是名称
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					if( HtmlStringTools::isJumpSpace( currentChar ) )
						break;
				}
			} else { // 如果是空，则找到一个非空
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					if( !HtmlStringTools::isJumpSpace( currentChar ) ) {
						++equIndex;
						for( ; equIndex < endIndex; ++equIndex ) { // 找到末尾的空，或者结束该节点
							currentChar = startWStrPtr[ equIndex ];
							if( HtmlStringTools::isJumpSpace( currentChar ) )
								break;
						}
						break;
					}
				}
			}
			break;
		}

	}
	std::pair< HtmlString, HtmlString > pairUnity;
	// index 的下标类型
	// 0 未定义下标，循环首次开始
	// 1 定义 key 的开始下标
	// 2 定义 value 的开始下标
	// 3 未定义 key 的开始下标（已经实现 key/value 的配对）
	int currentType = 0;
	auto node = htmldocShared->getWSNode( thisSharedPtr );
	std::vector< wchar_t > key, value;
	for( ; equIndex < endIndex; ++equIndex ) {
		currentChar = startWStrPtr[ equIndex ];
		// 找到等号位置
		if( currentType == 2 ) {
			HtmlString keyval( key.data( ), key.size( ) );
			HtmlString mapval( value.data( ), value.size( ) );
			key.clear( );
			value.clear( );
			result->insert_or_assign( keyval, mapval );
			if( currentChar == nodeEndChar )
				break;
			if( HtmlStringTools::isJumpSpace( currentChar ) )// 找到下一个非空
				for( ++equIndex; equIndex < endIndex; ++equIndex )
					if( HtmlStringTools::isJumpSpace( startWStrPtr[ equIndex ] ) )
						break;
			currentType = 1;
		} else {
			if( currentType == 0 )
				currentType = 1;
			if( currentChar == euq ) {
				if( currentType == 1 )
					currentType = 2;
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) { // 找到 = 的下一个非空
					currentChar = startWStrPtr[ equIndex ];
					if( !HtmlStringTools::isJumpSpace( currentChar ) ) {
						// 找到值的末尾
						for( ; equIndex < endIndex; ++equIndex ) {
							currentChar = startWStrPtr[ equIndex ];
							if( HtmlStringTools::isJumpSpace( currentChar ) || currentChar == nodeEndChar )
								break;
							else if( currentChar == singleQuotation ) {
								value.emplace_back( currentChar );
								++equIndex;
								for( ; equIndex < endIndex; ++equIndex ) {
									currentChar = startWStrPtr[ equIndex ];
									value.emplace_back( currentChar );
									if( currentChar == singleQuotation )
										break;
								}
							} else if( currentChar == doubleQuotation ) {
								value.emplace_back( currentChar );
								++equIndex;
								for( ; equIndex < endIndex; ++equIndex ) {
									currentChar = startWStrPtr[ equIndex ];
									value.emplace_back( currentChar );
									if( currentChar == doubleQuotation )
										break;
								}
							}
						}
						break;
					}
				}
			} else if( currentChar == singleQuotation ) {
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					key.emplace_back( currentChar );
					if( currentChar == singleQuotation )
						break;
				}
			} else if( currentChar == doubleQuotation ) {
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					key.emplace_back( currentChar );
					if( currentChar == doubleQuotation )
						break;
				}
			} else if( !HtmlStringTools::isJumpSpace( currentChar ) )
				key.emplace_back( currentChar );
		}

	}
	if( key.size( ) != 0 || value.size( ) != 0 ) {
		HtmlString keyval( key.data( ), key.size( ) );
		HtmlString mapval( value.data( ), value.size( ) );
		result->insert_or_assign( keyval, mapval );
	}
	*refNodeAttributes = *result;
	return result;
}
HtmlString_Shared HtmlNode::getWSNode( ) const {
	return htmldocShared->getWSNode( thisSharedPtr );
}
HtmlString_Shared HtmlNode::getNodeWSName( ) const {
	return htmldocShared->getNodeWSName( thisSharedPtr );
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
HtmlString_Shared HtmlNode::getContent( ) const {
	return htmldocShared->getContent( thisSharedPtr );
}
HtmlString_Shared HtmlNode::getPath( ) const {
	return htmldocShared->getPath( thisSharedPtr );
}
HtmlString_Shared HtmlNode::getNodeText( ) const {
	return htmldocShared->getNodeText( thisSharedPtr );
}
bool HtmlNode::findAttribute( const std::function< bool( const WStringPairUnorderMap_Shared ) > callFunction ) const {
	return htmldocShared->findAttribute( thisSharedPtr, callFunction );
}
Vector_HtmlNodeSPtr_Shared HtmlNode::xpath( const HtmlString &xpath ) {
	XPath xPath( xpath );
	return xPath.buider( { this->thisSharedPtr } );
}
Vector_HtmlNodeSPtr_Shared HtmlNode::getHtmlNodeRoots( ) {
	return htmldocShared->getHtmlNodeRoots( );
}
Vector_HtmlNodeSPtr_Shared HtmlNode::parseHtmlNodeCharPair( HtmlDoc_Shared html_doc_shared, size_t start_index, const size_t max_index, size_t &index_count ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	bool findCharResut = false;
	index_count = start_index;
	auto stdCWString = html_doc_shared->htmlWCStr;
	for( ; start_index < max_index; ++start_index ) {
		auto maxIndex = max_index;
		findCharResut = HtmlDoc::findNextNodeStartChar( stdCWString, maxIndex, start_index );
		if( !findCharResut )
			break;
		auto ptr = new HtmlNode;
		HtmlNode_Shared currentHtmlNodeCharPairSharedPtr( ptr );
		ptr->ptrOffset = start_index;
		findCharResut = HtmlDoc::findNextNodeEndChar( stdCWString, maxIndex, start_index );
		if( !findCharResut )
			break;
		ptr->ptrCWtrLen = start_index + 1 - ptr->ptrOffset;
		ptr->czWStr = stdCWString;
		ptr->htmldocShared = html_doc_shared;
		ptr->thisSharedPtr = currentHtmlNodeCharPairSharedPtr;
		result->emplace_back( currentHtmlNodeCharPairSharedPtr );
	}
	index_count = start_index - index_count;
	return result;
}

void HtmlNode::setParent( HtmlNode_Shared child, HtmlNode_Shared parent ) {

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
	parent->subChildren->emplace_back( child );
	child->parent = parent;
}
