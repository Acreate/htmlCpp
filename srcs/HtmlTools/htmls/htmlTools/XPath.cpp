#include "XPath.h"

#include <algorithm>
#include <iostream>


#include "XDir.h"
#include "../../htmlString/HtmlStringTools.h"
#include "../htmlNode/HtmlNode.h"
#include "../htmlDoc/HtmlDoc.h"

using namespace cylHtmlTools;
using namespace cylHtmlTools::charValue;
XPath::XPath( const HtmlString &wstr ) : XPath( ) {
	size_t length = wstr.length( );
	if( length == 0 || wstr.empty( ) )
		return;
	// 分割目录，并且存储到 dirListSPtr : XDir_Shared 列表但其中
	HtmlChar *buff = new HtmlChar[ length ];
	size_t buffIndex = 0;
	for( auto &pathChar : wstr ) {
		if( HtmlStringTools::isRouteChar( pathChar ) ) {
			if( buffIndex > 0 ) {
				HtmlString dirName( buff, buffIndex );
				dirListSPtr.emplace_back( std::make_shared< XDir >( dirName ) );
				buffIndex = 0;
				continue;
			} else if( dirListSPtr.size( ) == 0 ) {
				HtmlString dirName( 1, pathChar );
				dirListSPtr.emplace_back( std::make_shared< XDir >( dirName ) );
				buffIndex = 0;
				continue;
			}
			continue;
		}
		buff[ buffIndex ] = pathChar;
		++buffIndex;
	}
	if( buffIndex != 0 ) {
		HtmlString dirName( buff, buffIndex );
		dirListSPtr.emplace_back( std::make_shared< XDir >( dirName ) );
	}
	delete[] buff;
}
XPath::XPath( const List_HtmlStringSptr &std_w_string_list_shared, const HtmlString &separator )
	: separator( separator ) {
	for( auto &stdWString : std_w_string_list_shared )
		dirListSPtr.emplace_back( std::make_shared< XDir >( *stdWString ) );
}
XPath::~XPath( ) { }

static void findItem( Vector_HtmlNodeSPtr_Shared buff, XDir_Shared &subPath, Vector_HtmlNodeSPtr_Shared findNodes ) {
	auto nodeIterator = findNodes->begin( );
	auto nodeEnd = findNodes->end( );
	while( true ) {
		if( nodeIterator == nodeEnd )
			break;
		auto nodeWsName = *nodeIterator->get( )->getNodeName( );
		if( subPath->hasName( nodeWsName ) ) {
			HtmlStringPairUnorderMap_Shared analysisAttribute = nodeIterator->get( )->analysisAttribute( );
			if( subPath->hasAttribute( analysisAttribute, nodeWsName ) )
				buff->emplace_back( *nodeIterator );
		}
		++nodeIterator;
	}
}
Vector_HtmlNodeSPtr_Shared XPath::rootBuider( HtmlDoc_Shared &html_doc ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	// 获取所有根节点
	auto analysisBrotherNode = html_doc->analysisBrotherNode( ); // 必须解析族谱

	HtmlDoc *element = html_doc.get( );
	auto findNodes( std::make_shared< Vector_HtmlNodeSPtr >( ) );
	for( auto &node : *element->getHtmlNodeRoots( ) )
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
	if( index == strListSize )
		for( auto &node : *buff )
			result->emplace_back( node );
	return result;
}
Vector_HtmlNodeSPtr_Shared XPath::relativeBuider( HtmlNode_Shared &html_node_shared ) {
	Vector_HtmlNodeSPtr_Shared result( new Vector_HtmlNodeSPtr );
	// 获取所有根节点
	html_node_shared->analysisAttribute( ); // 必须解析族谱

	auto &findNodes = html_node_shared->subChildren;
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
	if( index == strListSize )
		for( auto &node : *buff )
			result->emplace_back( node );
	return result;
}
Vector_HtmlNodeSPtr_Shared XPath::anyBuider( Vector_HtmlNodeSPtr &html_node_shared ) {
	return { };
}
XPath::XPath( ) {

}

Vector_HtmlNodeSPtr XPath::pathControlDirName( Vector_HtmlNodeSPtr &current_find_nodes, XDir_Control_Type control_type ) {
	Vector_HtmlNodeSPtr result;
	switch( control_type ) {
	case Cd_Current :// 获取参数列表节点当中的友邻节点-如果存在
		for( auto &node : current_find_nodes ) {
			auto copyNode = node.get( );
			for( auto &resultNode : result )
				if( *resultNode.get( ) == *copyNode ) {
					copyNode = nullptr;
					break;
				}
			if( copyNode ) {
				result.emplace_back( node );
				auto brotherVSPtr = copyNode->getBrother( );
				for( auto &brother : *brotherVSPtr ) {
					copyNode = brother.get( );
					for( auto &resultNode : result )
						if( *resultNode.get( ) == *copyNode ) {
							copyNode = nullptr;
							break;
						}
					if( copyNode )
						result.emplace_back( brother );
				}
			}
		}
		break;
	case Cd_Parent : // 获取参数列表节点当中的上级节点-如果存在，则遍历友邻节点
		for( auto &node : current_find_nodes ) {
			auto htmlNode = node->getParent( ).get( );
			if( htmlNode ) {
				for( auto &resultNode : result )
					if( *resultNode.get( ) == *htmlNode ) {
						htmlNode = nullptr;
						break;
					}
				if( htmlNode ) {
					result.emplace_back( node );
					const auto &brothers = htmlNode->getBrother( );
					for( auto &brother : *brothers ) {
						htmlNode = brother.get( );
						for( auto &resultNode : result )
							if( *resultNode.get( ) == *htmlNode ) {
								htmlNode = nullptr;
								break;
							}
						if( htmlNode )
							result.emplace_back( brother );
					}
				}
			}
		}
		break;
	case Cd_Root : // 获取参数列表节点当中的根节点
		for( auto &node : current_find_nodes ) {
			auto htmlNodeRoots = node->getHtmlNodeRoots( );
			for( auto &rootNode : *htmlNodeRoots ) {
				auto copyNode = rootNode.get( );
				for( auto &resultNode : result )
					if( *resultNode.get( ) == *copyNode ) {
						copyNode = nullptr;
						break;
					}
				if( copyNode )
					result.emplace_back( rootNode );
			}
		}
		break;
	}
	return result;
}

Vector_HtmlNodeSPtr XPath::matchesHtmlDocAllNodes( Vector_HtmlNodeSPtr &currentFindNodes, XDir *x_dir, HtmlString &path ) {
	Vector_HtmlNodeSPtr buff;
	auto currentFindNodesBegin = currentFindNodes.begin( );
	auto currentFindNodesEnd = currentFindNodes.end( );
	for( ; currentFindNodesBegin != currentFindNodesEnd; ++currentFindNodesBegin ) {
		auto findNodes = currentFindNodesBegin->get( )->htmldocShared->findNodes( [&]( HtmlNode_Shared &node ) ->bool {
			auto nodeName = *node->getNodeName( );
			bool equRemoveSpaceOverHtmlString = HtmlStringTools::equRemoveSpaceOverHtmlString( nodeName, path );
			if( equRemoveSpaceOverHtmlString )
				if( x_dir->hasAttribute( node->analysisAttribute( ), nodeName ) )
					return true;
			return false;
		} );
		for( auto &findNode : *findNodes ) {
			auto buffIterator = buff.begin( );
			auto buffEnd = buff.end( );
			for( ; buffIterator != buffEnd; ++buffIterator )
				if( *buffIterator->get( ) == *findNode.get( ) )
					break;
			if( buffIterator == buffEnd )
				buff.emplace_back( findNode );
		}
	}
	return buff;
}

Vector_HtmlNodeSPtr_Shared XPath::buider( Vector_HtmlNodeSPtr_Shared &html_node_shared_s ) {
	auto iterator = dirListSPtr.begin( );
	auto end = dirListSPtr.end( );

	if( iterator == end ) // 根本没有节点
		return nullptr;


	Vector_HtmlNodeSPtr_Shared resultShared( new Vector_HtmlNodeSPtr );
	auto currentFindNodes = *html_node_shared_s;
	HtmlString rootPath( 1, forwardSlash );
	HtmlString currentPath( 1, dot );
	HtmlString topPath( 1, dot );
	HtmlString dirName;
	// 第一个节点
	auto xdir = iterator->get( );
	dirName.append( xdir->getDirName( ) );
	if( HtmlStringTools::equRemoveSpaceOverHtmlString( rootPath, dirName ) ) {
		currentFindNodes = pathControlDirName( currentFindNodes, Cd_Root );
	} else if( HtmlStringTools::equRemoveSpaceOverHtmlString( currentPath, dirName ) ) {
		currentFindNodes = pathControlDirName( currentFindNodes, Cd_Current );
	} else if( HtmlStringTools::equRemoveSpaceOverHtmlString( topPath, dirName ) ) {
		currentFindNodes = pathControlDirName( currentFindNodes, Cd_Parent );
	} else { // 正常名称
		currentFindNodes = matchesHtmlDocAllNodes( currentFindNodes, xdir, dirName );
	}

	++iterator; // 下一个节点
	for( auto &node : currentFindNodes ) {
		auto resultIterator = resultShared->begin( );
		auto resultEnd = resultShared->end( );
		for( ; resultIterator != resultEnd; ++resultIterator )
			if( *resultIterator->get( ) == *node )
				break;
		if( resultIterator == resultEnd )
			resultShared->emplace_back( node );
	}
	if( iterator == end )  // 无法继续下去
		return resultShared;

	currentFindNodes.clear( );
	for( auto &node : *resultShared ) { // 获取子节点
		for( auto &subNode : *node->getChildren( ) ) {
			auto finNodeBegin = currentFindNodes.begin( );
			auto finNodeEnd = currentFindNodes.end( );
			for( ; finNodeBegin != finNodeEnd; ++finNodeBegin )
				if( *finNodeBegin->get( ) == *subNode.get( ) )
					break;
			if( finNodeBegin == finNodeEnd )
				currentFindNodes.emplace_back( subNode );
		}
	}

	// todo : 节点路径遍历

	// 根据 dirListSPtr 操作列表
	do {
		resultShared->clear( );
		xdir = iterator->get( );
		dirName = xdir->getDirName( );
		if( HtmlStringTools::equRemoveSpaceOverHtmlString( rootPath, dirName ) ) {
			currentFindNodes = pathControlDirName( currentFindNodes, Cd_Root );
		} else if( HtmlStringTools::equRemoveSpaceOverHtmlString( currentPath, dirName ) ) {
			currentFindNodes = pathControlDirName( currentFindNodes, Cd_Current );
		} else if( HtmlStringTools::equRemoveSpaceOverHtmlString( topPath, dirName ) ) {
			currentFindNodes = pathControlDirName( currentFindNodes, Cd_Parent );
		} else { // 正常名称

		/*
		 *	todo :
		 *	1.遍历当前节点，匹配名称
		 *	2.是否跳出调用
		 *		A.跳出 -> 返回当前匹配的所有节点
		 *		B.继续 -> 获取所有子节点
		 */
			
			
			for( auto &node : *resultShared ) { // 获取子节点
				for( auto &subNode : *node->getChildren( ) ) {
					auto finNodeBegin = currentFindNodes.begin( );
					auto finNodeEnd = currentFindNodes.end( );
					for( ; finNodeBegin != finNodeEnd; ++finNodeBegin )
						if( *finNodeBegin->get( ) == *subNode.get( ) )
							break;
					if( finNodeBegin == finNodeEnd )
						currentFindNodes.emplace_back( subNode );
				}
			}
		}

		++iterator; // 下一个节点
		for( auto &node : currentFindNodes ) {
			auto resultIterator = resultShared->begin( );
			auto resultEnd = resultShared->end( );
			for( ; resultIterator != resultEnd; ++resultIterator )
				if( *resultIterator->get( ) == *node )
					break;
			if( resultIterator == resultEnd )
				resultShared->emplace_back( node );
		}
		if( iterator == end )  // 无法继续下去
			return resultShared;
		currentFindNodes.clear( );

	} while( true );
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

XPath::operator HtmlString( ) const {
	HtmlString result;
	auto iterator = dirListSPtr.begin( );
	auto end = dirListSPtr.end( );
	if( iterator != end ) {
		auto subStr = iterator->get( )->getXDirName( );
		if( subStr.length( ) == 1 ) {
			if( HtmlStringTools::isRouteChar( subStr[ 0 ] ) ) {
				++iterator;
				result = result.append( subStr );
				if( iterator == end )
					return result;
			}
		}
		do {
			subStr = iterator->get( )->getXDirName( );
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
