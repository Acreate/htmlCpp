#include "HtmlNode.h"


#include "../htmlDoc/HtmlDoc.h"
#include "../../wstr/WStrTools.h"
#include <sstream>

using namespace HtmlTools;

HtmlNode::HtmlNode( ) : parent( nullptr ), subChildren( new Vector_HtmlNodeSPtr ), brother( new Vector_HtmlNodeSPtr ), refNodeAttributes( new WStringPairUnorderMap ) {
}
HtmlNode::~HtmlNode( ) {
}

WStringPairUnorderMap_Shared HtmlNode::analysisAttribute( ) {
	if( nodeType == Html_Node_Type::DoubleNode && endNode.get( ) == this ) {
		endNode->refNodeAttributes = startNode->refNodeAttributes;
		return startNode->refNodeAttributes;
	}

	WStringPairUnorderMap_Shared result( new WStringPairUnorderMap );

	auto startWStrPtr = this->czWStr->c_str( ) + ptrOffset;
	size_t equIndex = 0, endIndex = ptrCWtrLen - 1;

	wchar_t currentChar = L'\0';
	for( ; equIndex < endIndex; ++equIndex ) {
		currentChar = startWStrPtr[ equIndex ];
		if( currentChar == nodeStartChar ) { // 找到名称
			++equIndex;
			currentChar = startWStrPtr[ equIndex ];
			if( !WStrTools::isJumpSpace( currentChar ) ) { // 不是空，则是名称
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					if( WStrTools::isJumpSpace( currentChar ) )
						break;
				}
			} else { // 如果是空，则找到一个非空
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					if( !WStrTools::isJumpSpace( currentChar ) ) {
						++equIndex;
						for( ; equIndex < endIndex; ++equIndex ) { // 找到末尾的空，或者结束该节点
							currentChar = startWStrPtr[ equIndex ];
							if( WStrTools::isJumpSpace( currentChar ) )
								break;
						}
						break;
					}
				}
			}
			break;
		}

	}
	std::pair< std::wstring, std::wstring > pairUnity;
	size_t keyIndex = 0, keyLen = 0, valueIndex = 0, valueLen = 0;
	// index 的下标类型
	// 0 未定义下标，循环首次开始
	// 1 定义 key 的开始下标
	// 2 定义 value 的开始下标
	// 3 未定义 key 的开始下标（已经实现 key/value 的配对）
	int currentType = 0;
	for( ; equIndex < endIndex; ++equIndex ) {
		currentChar = startWStrPtr[ equIndex ];
		// 找到等号位置
		if( currentType == 2 && WStrTools::isJumpSpace( currentChar ) ) {
			valueLen = equIndex - valueIndex;
			std::wstring keyval( startWStrPtr + keyIndex, keyLen );
			std::wstring mapval( startWStrPtr + valueIndex, valueLen );
			result->insert_or_assign( keyval, mapval );
			++equIndex;
			for( ; equIndex < endIndex; ++equIndex ) {
				currentChar = startWStrPtr[ equIndex ];
				if( !WStrTools::isJumpSpace( currentChar ) ) {
					keyIndex = equIndex;
					--equIndex;
					break;
				}
			}
			currentType = 1;
		} else {
			if( currentType == 0 ) {
				keyIndex = equIndex;
				currentType = 1;
			}
			if( currentChar == euq ) {
				if( currentType == 1 ) {
					currentType = 2;
					keyLen = equIndex - keyIndex;
				}
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) { // 找到 = 的下一个非空
					currentChar = startWStrPtr[ equIndex ];
					if( !WStrTools::isJumpSpace( currentChar ) ) {
						valueIndex = equIndex;
						--equIndex;
						break;
					}
				}
			} else if( currentChar == singleQuotation ) {
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					if( currentChar == singleQuotation ) {
						break;
					}
				}
			} else if( currentChar == doubleQuotation ) {
				++equIndex;
				for( ; equIndex < endIndex; ++equIndex ) {
					currentChar = startWStrPtr[ equIndex ];
					if( currentChar == doubleQuotation ) {
						break;
					}
				}
			}
		}

	}

	*refNodeAttributes = *result;
	return result;
}
Vector_HtmlNodeSPtr_Shared HtmlNode::parseHtmlNodeCharPair( std::shared_ptr< std::wstring > std_c_w_str, size_t start_index, const size_t max_index, size_t &index_count ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	bool findCharResut = false;
	index_count = start_index;

	for( ; start_index < max_index; ++start_index ) {
		auto maxIndex = max_index;
		findCharResut = HtmlDoc::findNextNodeStartChar( std_c_w_str, maxIndex, start_index );
		if( !findCharResut )
			break;
		auto ptr = new HtmlNode;
		HtmlNode_Shared currentHtmlNodeCharPairSharedPtr( ptr );
		ptr->ptrOffset = start_index;
		findCharResut = HtmlDoc::findNextNodeEndChar( std_c_w_str, maxIndex, start_index );
		if( !findCharResut )
			break;
		ptr->ptrCWtrLen = start_index + 1 - ptr->ptrOffset;
		ptr->czWStr = std_c_w_str;
		currentHtmlNodeCharPairSharedPtr->thisSharedPtr = currentHtmlNodeCharPairSharedPtr;
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
