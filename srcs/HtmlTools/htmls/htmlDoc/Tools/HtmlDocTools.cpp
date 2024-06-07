#include "HtmlDocTools.h"
#include "../../../htmlString/HtmlStringTools.h"
#include "../HtmlDoc.h"
#include "../../htmlNode/HtmlNode.h"
#include <sstream>
#include <iostream>

#include <string>
#include <codecvt>
#include <locale>
using namespace cylHtmlTools;
#define out_buff_debug
bool HtmlDocTools::findNodeName( const HtmlChar *std_c_w_string, size_t start_index, size_t end_index, HtmlString *result ) {
	HtmlChar currentChar = std_c_w_string[ end_index ];
	if( currentChar != charValue::nodeEndChar )
		for( --end_index; start_index < end_index; --end_index )
			if( std_c_w_string[ end_index ] == charValue::nodeEndChar )
				break;

	currentChar = std_c_w_string[ start_index ];
	if( currentChar != charValue::nodeStartChar )
		for( ++start_index; start_index < end_index; ++start_index )
			if( std_c_w_string[ start_index ] == charValue::nodeStartChar )
				break;

	if( start_index >= end_index ) // 错误的匹配
		return false;

	// 查找第一个非空
	for( ++start_index; start_index < end_index; ++start_index )
		if( !HtmlStringTools::isSpace( std_c_w_string[ start_index ] ) )
			break;

	size_t buffLen = end_index - start_index;
	HtmlChar *strBuff = new HtmlChar[ buffLen ];
	buffLen = 0; // 下标
	for( ; start_index < end_index; ++start_index ) {
		currentChar = std_c_w_string[ start_index ];
		if( HtmlStringTools::isSpace( currentChar ) || currentChar == charValue::forwardSlash )
			break;
		strBuff[ buffLen ] = currentChar;
		++buffLen;
	}
	*result = HtmlString( strBuff, buffLen );
	delete[] strBuff;
	return true;
}
bool HtmlDocTools::findNextNodeEndChar( const HtmlChar *std_c_w_string, const size_t max_index, size_t &start_index ) {
	for( ; start_index < max_index; ++start_index ) {
		auto currenChar = std_c_w_string[ start_index ];
		if( HtmlStringTools::isQuotation( currenChar ) ) {
			if( !HtmlStringTools::jumpQuotation( std_c_w_string, max_index, start_index, start_index ) )
				return false; // 不匹配
		} else if( currenChar == charValue::nodeEndChar )
			return true;
	}
	return false;
}
bool HtmlDocTools::findNextNodeStartChar( const HtmlChar *std_c_w_string, size_t max_index, size_t &start_index ) {
	for( ; start_index < max_index; ++start_index ) {
		auto currenChar = std_c_w_string[ start_index ];
		if( currenChar == charValue::nodeStartChar )
			return true;
	}
	return false;
}
bool HtmlDocTools::findNextNodeForwardSlash( const HtmlChar *std_c_w_string, const size_t max_index, size_t &start_index ) {
	for( ; start_index < max_index; ++start_index ) {
		auto currenChar = std_c_w_string[ start_index ];
		if( currenChar == charValue::forwardSlash )
			return true;
	}
	return false;
}
bool HtmlDocTools::isSingelNode( const HtmlChar *std_c_w_string, size_t &start_index, size_t &end_index ) {
	auto currentChar = std_c_w_string[ start_index ];
	if( currentChar != charValue::nodeStartChar )
		for( ++start_index; start_index < end_index; ++start_index ) {
			currentChar = std_c_w_string[ start_index ];
			if( HtmlStringTools::isQuotation( currentChar ) )
				if( HtmlStringTools::jumpQuotation( std_c_w_string, end_index, start_index, start_index ) )
					continue;
				else
					return false; // 找不到
			if( currentChar == charValue::nodeStartChar )
				break;
		}
	auto forwardSlashIndex = start_index + 1;
	if( findNextNodeForwardSlash( std_c_w_string, end_index, forwardSlashIndex ) ) {
		for( forwardSlashIndex += 1; forwardSlashIndex < end_index; ++forwardSlashIndex ) {
			currentChar = std_c_w_string[ forwardSlashIndex ];
			if( HtmlStringTools::isSpace( currentChar ) )
				continue;
			if( HtmlStringTools::isQuotation( currentChar ) )
				if( HtmlStringTools::jumpQuotation( std_c_w_string, end_index, start_index, start_index ) )
					continue;
				else
					return false; // 找不到
			if( currentChar != charValue::nodeEndChar )
				break;
			end_index = forwardSlashIndex;
			return true;
		}
	}
	return false;
}
bool HtmlDocTools::isStartNode( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index ) {
	auto c_str = std_c_w_string->c_str( );
	auto currentChar = c_str[ start_index ];
	if( currentChar != charValue::nodeStartChar )
		for( ++start_index; start_index < end_index; ++start_index ) {
			currentChar = c_str[ start_index ];
			if( HtmlStringTools::isQuotation( currentChar ) )
				if( HtmlStringTools::jumpQuotation( c_str, end_index, start_index, start_index ) )
					continue;
				else
					return false; // 找不到
			if( currentChar == charValue::nodeStartChar )
				break;
		}

	// 碰到的第一个必须是 > 而不是 /
	for( auto index = start_index + 1; index <= end_index; ++index ) {
		currentChar = c_str[ index ];
		if( HtmlStringTools::isSpace( currentChar ) )
			continue;
		if( HtmlStringTools::isQuotation( currentChar ) ) {
			if( !HtmlStringTools::jumpQuotation( c_str, end_index + 1, start_index, start_index ) )
				return false; // 不匹配
		} else if( currentChar == charValue::forwardSlash )
			return false;
		else {
			return true;
		}
	}
	return false;
}
bool HtmlDocTools::isEndNode( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index ) {
	auto c_str = std_c_w_string->c_str( );
	wchar_t currentChar = 0;
	// 碰到的第一个必须是 / 而不是通用字符或者 >
	for( ; start_index <= end_index; ++start_index ) {
		currentChar = c_str[ start_index ];
		if( HtmlStringTools::isQuotation( currentChar ) )
			if( HtmlStringTools::jumpQuotation( c_str, end_index, start_index, start_index ) )
				continue;
			else
				return false; // 找不到
		if( currentChar == charValue::nodeStartChar )
			break;
	}
	for( auto index = start_index + 1; index <= end_index; ++index ) {
		currentChar = c_str[ index ];
		if( HtmlStringTools::isQuotation( currentChar ) )
			if( HtmlStringTools::jumpQuotation( c_str, end_index, start_index, start_index ) )
				continue;
			else
				return false; // 找不到
		if( HtmlStringTools::isSpace( currentChar ) )
			continue;
		if( currentChar != charValue::forwardSlash )
			return true;
	}
	return false;
}
bool HtmlDocTools::isAnnotation( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index ) {
	auto c_str = std_c_w_string->c_str( );
	auto currentChar = c_str[ start_index ];
	if( currentChar != charValue::nodeStartChar )
		for( ++start_index; start_index < end_index; ++start_index ) {
			currentChar = c_str[ start_index ];
			if( HtmlStringTools::isQuotation( currentChar ) )
				if( HtmlStringTools::jumpQuotation( c_str, end_index, start_index, start_index ) )
					continue;
				else
					return false; // 找不到
			if( currentChar == charValue::nodeStartChar )
				break;
		}
	size_t endIndex = start_index + 1;

	for( ; endIndex < end_index; ++endIndex ) {
		currentChar = c_str[ endIndex ];
		if( HtmlStringTools::isQuotation( currentChar ) )
			if( HtmlStringTools::jumpQuotation( c_str, end_index, start_index, start_index ) )
				continue;
			else
				return false; // 找不到
		if( currentChar == charValue::exclamation ) {
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

void out_node_info( cylHtmlTools::HtmlNode_Shared &htmlNode, const cylHtmlTools::HtmlString &comp_name, const cylHtmlTools::HtmlString &title ) {

#ifdef  out_buff_debug
	auto nodeName = *htmlNode->getNodeName( );
	if( !comp_name.empty( ) && !HtmlStringTools::equRemoveSpaceOverHtmlString( comp_name, nodeName ) )
		return;
	std::wcout << L"============ " << title << "================" << std::endl;
	std::wcout << L"============================\t" << nodeName << std::endl;
	std::wcout << *htmlNode->getIncludeNodeContent( ) << std::endl;
	std::wcout << L"============================" << std::endl;
#endif
}

Vector_HtmlNodeSPtr_Shared HtmlDocTools::analysisDoubleNode( const HtmlDoc_Shared &html_doc_shared, HtmlNode_Shared html_start_node, Vector_HtmlNodeSPtr_Shared &html_node_char_pairs, size_t &start_index, size_t &end_index ) {

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
		result->emplace_back( startNode );
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
HtmlString_Shared HtmlDocTools::htmlStringContentTextConverToHtmlString( const HtmlChar *conver_contents, const size_t str_max_len ) {
	HtmlChar *buff = new HtmlChar[ str_max_len ];
	size_t buffIndex = 0;
	size_t index = 0; // 末尾下标
	size_t startPtr = 0; // 当前处理下标
	HtmlString nodeName; // 节点名称
	HtmlChar currentChar;
	do {
		currentChar = conver_contents[ index ];
		if( currentChar != charValue::nodeStartChar ) {
			buff[ buffIndex ] = currentChar;
			++buffIndex;
		} else {
			startPtr = index;
			for( ++index; index < str_max_len; ++index ) {
				currentChar = conver_contents[ index ];
				if( currentChar == charValue::nodeEndChar )
					break;
				if( HtmlStringTools::isQuotation( currentChar ) )
					if( !HtmlStringTools::jumpQuotation( conver_contents, str_max_len, index, index ) )
						return nullptr;
			}
			if( HtmlDocTools::findNodeName( conver_contents, startPtr, index, &nodeName )
				&& HtmlStringTools::equRemoveSpaceOverHtmlString( nodeName, L"br" ) ) {
				buff[ buffIndex ] = L'\n';
				++buffIndex;
			}
		}
		++index;
	} while( index < str_max_len );

	auto result = std::make_shared< HtmlString >( buff, buffIndex );
	delete[] buff;
	return result;

}
std::string  HtmlDocTools::converToStdString(const HtmlString& txt){
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(txt);
}
size_t HtmlDocTools::setFileAllWString( const cylHtmlTools::HtmlString &path, const cylHtmlTools::HtmlString &stringstream, std::ios_base::openmode open_mode) {
	std::wofstream ofstream;
	ofstream.open( converToStdString( path), open_mode );
	if( !ofstream.is_open( ) )
		return 0;
	auto &&write = ofstream.write( stringstream.c_str( ), stringstream.size( ) );
	auto tellp = write.tellp( );
	ofstream.close( );
	return tellp;
}
HtmlString_Shared HtmlDocTools::getFileAllWString( const cylHtmlTools::HtmlString &path, std::ios_base::openmode open_mode) {
	std::wstringstream stringstream;
	std::wifstream ifstream(  converToStdString( path), open_mode);
	if( !ifstream.is_open( ) )
		return nullptr;
	auto result = std::make_shared< HtmlString >( );
	do {
		auto &getline = std::getline( ifstream, *result );
		if( getline.eof( ) )
			break;
		if( !result->empty( ) )
			stringstream << *result;
		stringstream << L'\n';
		result->clear( );
	} while( true );
	ifstream.close( );
	*result = stringstream.str( );
	return result;

}
size_t HtmlDocTools::setFileAllWString( const std::string &path, const HtmlString &stringstream, std::ios_base::openmode open_mode ) {
	std::wofstream ofstream;
	ofstream.open( path, open_mode );
	if( !ofstream.is_open( ) || stringstream.empty( ) )
		return 0;
	auto &&write = ofstream.write( stringstream.c_str( ), stringstream.size( ) );
	auto tellp = write.tellp( );
	ofstream.flush( );
	ofstream.close( );
	return tellp;
}
HtmlString_Shared HtmlDocTools::getFileAllWString( const std::string &path, std::ios_base::openmode open_mode) {
	std::wifstream ifstream( path, open_mode );
	if( !ifstream.is_open( ) )
		return nullptr;
	std::wstringstream stringstream;
	auto result = std::make_shared< HtmlString >( );
	do {
		auto &getline = std::getline( ifstream, *result );
		if( !result->empty( ) )
			stringstream << *result;
		stringstream << L'\n';
		result->clear( );
		if( getline.eof( ) )
			break;
	} while( true );
	ifstream.close( );
	*result = stringstream.str( );
	return result;
}
