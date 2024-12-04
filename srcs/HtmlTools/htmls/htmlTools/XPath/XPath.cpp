#include "XPath.h"

#include <algorithm>
#include <iostream>

#include "../XDir/XDir.h"
#include "../../../htmlString/HtmlStringTools.h"
#include "../../htmlNode/HtmlNode.h"
#include "../../htmlDoc/HtmlDoc.h"

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
				dirListSPtr->emplace_back( std::make_shared< XDir >( dirName ) );
				buffIndex = 0;
				continue;
			} else if( dirListSPtr->size( ) == 0 ) {
				HtmlString dirName( 1, pathChar );
				dirListSPtr->emplace_back( std::make_shared< XDir >( dirName ) );
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
		dirListSPtr->emplace_back( std::make_shared< XDir >( dirName ) );
	}
	delete[] buff;
}
XPath::XPath( const List_HtmlStringSptr &std_w_string_list_shared, const HtmlString &separator )
	: separator( separator ), dirListSPtr( std::make_shared< Vector_XDirSPtr >( ) ) {
	for( auto &stdWString : std_w_string_list_shared )
		dirListSPtr->emplace_back( std::make_shared< XDir >( *stdWString ) );
}
XPath::~XPath( ) { }

XPath::XPath( ): dirListSPtr( std::make_shared< Vector_XDirSPtr >( ) ) {

}

inline bool hasNode( const Vector_HtmlNodeSPtr &checkList, const HtmlNode_Shared &node ) {
	auto iterator = checkList.begin( );
	auto end = checkList.end( );
	for( ; iterator != end; ++iterator )
		if( *iterator->get( ) == *node )
			return true;
	return false;
}

Vector_HtmlNodeSPtr XPath::pathControlDirName( const Vector_HtmlNodeSPtr &current_find_nodes, const XDir *xdirInfo, XDir_Control_Type current_control_type, XDir_Control_Type old_control_type ) const {
	Vector_HtmlNodeSPtr result;
	switch( current_control_type ) {
		case Cd_Current :// 获取参数列表节点当中的所有子节点-如果存在
			for( auto &node : current_find_nodes ) {
				//// 跳过尾节点
				if( node->isEndNode( ) )
					continue;
				Vector_HtmlNodeSPtr_Shared children = node->getChildren( );
				for( auto &childerNode : *children ) {
					if( childerNode->isEndNode( ) )
						continue;
					if( !hasNode( result, childerNode ) )
						result.emplace_back( childerNode );
				}
			}
			break;
		case Cd_Parent : // 获取参数列表节点当中的上级节点
			for( auto &node : current_find_nodes ) {
				// 跳过尾节点
				if( node->isEndNode( ) )
					continue;
				HtmlNode_Shared parent = node->getParent( );
				if( parent->isEndNode( ) )
					continue;
				if( parent && parent->isStartNode( ) )
					result.emplace_back( parent );
			}
			break;
		case Cd_Root : // 获取参数列表节点当中的根节点
			for( auto &node : current_find_nodes ) {
				auto htmlNodeRoots = node->getHtmlNodeRoots( );
				for( auto &rootNode : *htmlNodeRoots ) {
					auto copyNode = rootNode.get( );
					// 跳过尾节点
					if( copyNode->isEndNode( ) )
						continue;
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
		case Cd_None : {
			if( old_control_type == Cd_None ) // 上次节点控制符使用的是名称代表，则当前需要使用子目录
				for( auto &findNode : current_find_nodes ) { // 获取检查节点
					auto copyNode = findNode.get( );
					// 跳过尾节点
					if( copyNode->isEndNode( ) )
						continue;
					for( auto &subNode : *findNode->getChildren( ) ) { // 获取子节点
						auto iterator = result.begin( );
						auto end = result.end( );
						for( ; iterator != end; ++iterator )
							if( *iterator->get( ) == *subNode.get( ) )
								break;
						if( iterator == end ) {
							auto converXDirSptr = subNode->converXDirSptr( );
							if( converXDirSptr )
								if( converXDirSptr->isLeftXDirIncludeRightXDir( converXDirSptr.get( ), xdirInfo ) )
									result.emplace_back( subNode );
						}
					}
				}
			else { // 上次节点控制符使用的是路径控制符，则当前需要使用当前目录
				for( auto &findNode : current_find_nodes ) { // 获取检查节点
					auto copyNode = findNode.get( );
					// 跳过尾节点
					if( copyNode->isEndNode( ) )
						continue;
					auto iterator = result.begin( );
					auto end = result.end( );
					for( ; iterator != end; ++iterator )
						if( *iterator->get( ) == *findNode.get( ) )
							break;
					if( iterator == end ) {
						auto converXDirSptr = findNode->converXDirSptr( );
						if( converXDirSptr->isLeftXDirIncludeRightXDir( converXDirSptr.get( ), xdirInfo ) )
							result.emplace_back( findNode );
					}
				}
			}
		}
		break;
	}

	Vector_HtmlNodeSPtr buff;
	for( auto &node : result ) {
		auto copyNode = node.get( );
		// 跳过尾节点
		if( copyNode->isEndNode( ) )
			continue;
		buff.emplace_back( node );
	}
	return buff;
}

Vector_HtmlNodeSPtr XPath::matchesHtmlDocAllNodes( const Vector_HtmlNodeSPtr &currentFindNodes, const XDir *x_dir, const HtmlString &path ) const {
	Vector_HtmlNodeSPtr buff;
	auto currentFindNodesBegin = currentFindNodes.begin( );
	auto currentFindNodesEnd = currentFindNodes.end( );
	for( ; currentFindNodesBegin != currentFindNodesEnd; ++currentFindNodesBegin ) {
		auto findNodes = currentFindNodesBegin->get( )->htmldocShared->findNodes( [&]( HtmlNode_Shared &node ) ->bool {
			if( node->isEndNode( ) ) // 跳过尾节点
				return false;
			/*auto name = node->getNodeName( );
			if( HtmlStringTools::equRemoveSpaceOverHtmlString( L"div", *name ) ) {
				auto unorderedMap = node->findAttribute( []( const HtmlString &attributeName, const HtmlString &attributeValue ) {
					size_t index = 0;
					HtmlString fin = L"sitebox";
					if( HtmlStringTools::findNextHtmlStringPotion( &attributeValue, 0, &fin, index, &index ) )
						return true;
					return false;
				} );
				if( unorderedMap ) {
					auto content = node->getIncludeNodeContent( );
					std::wcout << "\n===================\t" << *name << '\n';
					std::wcout << *content;
					std::wcout << "\n===================\t" << *name << '\n' << std::endl;
				}

			}*/
			auto analysisAttribute = node->converXDirSptr( );
			if( analysisAttribute && x_dir->isOtherXDirIncludeThisXDir( analysisAttribute ) )
				return true;
			return false;
		} );
		if( findNodes )
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
HtmlString XPath::getHtmlString( ) const {
	HtmlString result;
	auto iterator = dirListSPtr->begin( );
	auto end = dirListSPtr->end( );
	if( iterator == end )
		return result;
	do {
		auto subStr = iterator->get( )->getXDirName( );
		++iterator;
		if( iterator == end ) {
			result = result.append( subStr );
			break;
		}
		result = result.append( subStr ) + charValue::forwardSlash;
	} while( true );

	return result;
}
Vector_HtmlNodeSPtr_Shared XPath::buider( const Vector_HtmlNodeSPtr_Shared &html_node_shared_s ) const {
	auto iterator = dirListSPtr->begin( );
	auto end = dirListSPtr->end( );
	if( iterator == end ) // 根本没有节点
		return nullptr;
	Vector_HtmlNodeSPtr_Shared resultShared( new Vector_HtmlNodeSPtr );
	auto currentFindNodes = *html_node_shared_s;
	HtmlString rootPath( 1, forwardSlash );
	HtmlString currentPath( 1, dot );
	HtmlString topPath( 1, dot );
	HtmlString dirName;
	auto dirControlType = Cd_None;
	// 第一个节点
	auto xdir = iterator->get( );
	dirName.append( xdir->getDirName( ) );
	if( HtmlStringTools::equRemoveSpaceOverHtmlString( rootPath, dirName ) ) {
		currentFindNodes = pathControlDirName( currentFindNodes, xdir, Cd_Root, dirControlType );
		dirControlType = Cd_Root;
	} else if( HtmlStringTools::equRemoveSpaceOverHtmlString( currentPath, dirName ) ) {
		currentFindNodes = pathControlDirName( currentFindNodes, xdir, Cd_Current, dirControlType );
		dirControlType = Cd_Current;
	} else if( HtmlStringTools::equRemoveSpaceOverHtmlString( topPath, dirName ) ) {
		currentFindNodes = pathControlDirName( currentFindNodes, xdir, Cd_Parent, dirControlType );
		dirControlType = Cd_Parent;
	} else { // 正常名称
		currentFindNodes = matchesHtmlDocAllNodes( currentFindNodes, xdir, dirName );
		dirControlType = Cd_None;
	}
	if( currentFindNodes.size( ) == 0 ) // 没有找到任何节点
		return nullptr;
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

	// 根据 dirListSPtr 操作列表
	do {
		resultShared->clear( );
		xdir = iterator->get( );
		dirName = xdir->getDirName( );
		if( HtmlStringTools::equRemoveSpaceOverHtmlString( rootPath, dirName ) ) {
			currentFindNodes = pathControlDirName( currentFindNodes, xdir, Cd_Root, dirControlType );
			dirControlType = Cd_Root;
		} else if( HtmlStringTools::equRemoveSpaceOverHtmlString( currentPath, dirName ) ) {
			currentFindNodes = pathControlDirName( currentFindNodes, xdir, Cd_Current, dirControlType );
			dirControlType = Cd_Current;
		} else if( HtmlStringTools::equRemoveSpaceOverHtmlString( topPath, dirName ) ) {
			currentFindNodes = pathControlDirName( currentFindNodes, xdir, Cd_Parent, dirControlType );
			dirControlType = Cd_Parent;
		} else { // 正常名称
			currentFindNodes = pathControlDirName( currentFindNodes, xdir, Cd_None, dirControlType );
			dirControlType = Cd_None;
		}
		if( currentFindNodes.size( ) == 0 ) // 没有找到任何节点
			return nullptr;
		++iterator;
		if( iterator == end )
			break;

	} while( true );
	if( currentFindNodes.size( ) == 0 )
		return nullptr;
	resultShared->clear( );
	for( auto &node : currentFindNodes ) {
		auto resultIterator = resultShared->begin( );
		auto resultEnd = resultShared->end( );
		for( ; resultIterator != resultEnd; ++resultIterator )
			if( *resultIterator->get( ) == *node )
				break;
		if( resultIterator == resultEnd )
			resultShared->emplace_back( node );
	}
	return resultShared;
}
