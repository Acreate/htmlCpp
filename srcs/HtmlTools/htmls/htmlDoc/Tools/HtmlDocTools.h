#ifndef HTMLDOCTOOLS_H_H_HEAD__FILE__
#define HTMLDOCTOOLS_H_H_HEAD__FILE__
#pragma once
#include <ios>
#include <fstream>
#include "../../../nameSpace/cylHtmlTools.h"

#include "../../enum/HtmlNodeType/Html_Node_Type.h"

namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT HtmlDocTools {
	public: // 友元
		friend class HtmlNode;
		friend class XPath;
		friend class HtmlDoc;
	public:
		/// <summary>
		/// 查找节点名称
		/// </summary>
		/// <param name="std_c_w_string">匹配的字符串</param>
		/// <param name="start_index">开始下标，它总是指向 '<' 符号，否则发生向后查找事件</param>
		/// <param name="end_index">结束下标，它总是指向 '>' 符号，否则发生向前查找事件</param>
		/// <param name="result">返回字符串</param>
		/// <returns>存在返回 true</returns>
		static bool findNodeName( const HtmlChar *std_c_w_string, size_t start_index, size_t end_index, HtmlString *result );
		/// <summary>
		/// 查找节点名称
		/// </summary>
		/// <param name="std_c_w_string">匹配的字符串</param>
		/// <param name="start_index">开始下标，它总是指向 '<' 符号，否则发生向后查找事件</param>
		/// <param name="end_index">结束下标，它总是指向 '>' 符号，否则发生向前查找事件</param>
		/// <param name="result">返回字符串</param>
		/// <returns>存在返回 true</returns>
		static bool findNodeName( const HtmlString *std_c_w_string, size_t start_index, size_t end_index, HtmlString *result ) {
			return findNodeName( std_c_w_string->c_str( ), start_index, end_index, result );
		}
		/// <summary>
		/// 查找节点名称
		/// </summary>
		/// <param name="std_c_w_string">匹配的字符串</param>
		/// <param name="start_index">开始下标，它总是指向 '<' 符号，否则发生向后查找事件</param>
		/// <param name="end_index">结束下标，它总是指向 '>' 符号，否则发生向前查找事件</param>
		/// <param name="result">返回字符串</param>
		/// <returns>存在返回 true</returns>
		static bool findNodeName( const HtmlString_Shared &std_c_w_string, size_t start_index, size_t end_index, HtmlString *result ) {
			return findNodeName( std_c_w_string.get( ), start_index, end_index, result );
		}
		/// <summary>
		/// 查找下一个节点结束符的位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeEndChar( const HtmlChar *std_c_w_string, const size_t max_index, size_t &start_index );
		/// <summary>
		/// 查找下一个节点结束符的位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeEndChar( const HtmlString_Shared &std_c_w_string, const size_t max_index, size_t &start_index ) {
			return findNextNodeEndChar( std_c_w_string->c_str( ), max_index, start_index );
		}
		/// <summary>
		/// 查找下一个节点结束符的位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeEndChar( const HtmlString_Shared &std_c_w_string, size_t &start_index ) {
			return findNextNodeEndChar( std_c_w_string->c_str( ), std_c_w_string->length( ), start_index );
		}
		/// <summary>
		/// 查找下一个节点开始符的位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeStartChar( const HtmlChar *std_c_w_string, const size_t max_index, size_t &start_index );
		/// <summary>
		/// 查找下一个节点开始符的位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeStartChar( const HtmlString_Shared &std_c_w_string, const size_t max_index, size_t &start_index ) {
			return findNextNodeStartChar( std_c_w_string->c_str( ), max_index, start_index );
		}
		/// <summary>
		/// 查找下一个节点开始符的位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeStartChar( const HtmlString_Shared &std_c_w_string, size_t &start_index ) {
			return findNextNodeStartChar( std_c_w_string->c_str( ), std_c_w_string->length( ), start_index );
		}

		/// <summary>
		/// 查找下一个节点的斜杠位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeForwardSlash( const HtmlChar *std_c_w_string, const size_t max_index, size_t &start_index );
		/// <summary>
		/// 查找下一个节点的斜杠位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeForwardSlash( const HtmlString_Shared &std_c_w_string, const size_t max_index, size_t &start_index ) {
			return findNextNodeForwardSlash( std_c_w_string->c_str( ), max_index, start_index );
		}
		/// <summary>
		/// 判断节点是否为单节点
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向节点开始字符 '<'，如果不是，那么它将会向后查询</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示单下标</returns>
		static bool isSingelNode( const HtmlChar *std_c_w_string, size_t &start_index, size_t &end_index );
		/// <summary>
		/// 判断节点是否为单节点
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向节点开始字符 '<'，如果不是，那么它将会向后查询</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示单下标</returns>
		static bool isSingelNode( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index ) {
			return isSingelNode( std_c_w_string->c_str( ), start_index, end_index );
		}
		/// <summary>
		/// 判断节点是否为开始节点
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向 '<'</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示单下标</returns>
		static bool isStartNode( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index );
		/// <summary>
		/// 判断节点是否为结束节点
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向 '<'</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示单下标</returns>
		static bool isEndNode( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index );
		/// <summary>
		/// 判断节点是否为注释节点(包含 !DOCTYPE 节点)
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向节点开始字符 '<'，如果不是，那么它将会向后查询</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示注释节点</returns>
		static bool isAnnotation( const HtmlString_Shared &std_c_w_string, size_t &start_index, size_t &end_index );
		/// <summary>
		/// 解析双节点，匹配第一个指向的节点。
		/// </summary>
		/// <param name="html_doc_shared">引用节点</param>
		/// <param name="html_start_node">开始节点</param>
		/// <param name="html_node_char_pairs">节点列表</param>
		/// <param name="start_index">开始的节点列表下标</param>
		/// <param name="end_index">结束的节点列表下标</param>
		/// <returns>匹配的节点列表</returns>
		static Vector_HtmlNodeSPtr_Shared analysisDoubleNode( const HtmlDoc_Shared &html_doc_shared, HtmlNode_Shared html_start_node, Vector_HtmlNodeSPtr_Shared &html_node_char_pairs, size_t &start_index, size_t &end_index );
		/// <summary>
		/// 把一个节点内容当中的文本转换到文本
		/// </summary>
		/// <param name="conver_contents">转换文本</param>
		/// <param name="str_max_len">文本长度</param>
		/// <returns>转换后的文本</returns>
		static HtmlString_Shared htmlStringContentTextConverToHtmlString( const HtmlChar *conver_contents, const size_t str_max_len );
		/// <summary>
		/// 把一个节点内容当中的文本转换到文本
		/// </summary>
		/// <param name="conver_contents">节点内容的转换</param>
		/// <returns>文本内容</returns>
		static HtmlString_Shared htmlStringContentTextConverToHtmlString( const HtmlString_Shared &conver_contents ) {
			return htmlStringContentTextConverToHtmlString( conver_contents->c_str( ), conver_contents->length( ) );
		}
		static HtmlString_Shared htmlStringContentTextConverToHtmlString( const cylHtmlTools::HtmlString &conver_contents ) {
			return htmlStringContentTextConverToHtmlString( conver_contents.c_str( ), conver_contents.length( ) );
		}
		/// <summary>
		/// 写入文件
		/// </summary>
		/// <param name="path">文件路径</param>
		/// <param name="stringstream">文件内容</param>
		/// <param name="open_mode">打开模式</param>
		/// <param name="open_prot">属性</param>
		/// <returns>写入数量</returns>
		static size_t setFileAllWString( const HtmlString &path, const HtmlString &stringstream, std::ios_base::openmode open_mode = std::ios::binary | std::ios::out | std::ios::trunc );

		/// <summary>
		/// 读取文件
		/// </summary>
		/// <param name="path">文件路径</param>
		/// <param name="open_mode">打开模式</param>
		/// <param name="open_prot">属性</param>
		/// <returns>文件内容</returns>
		static HtmlString_Shared getFileAllWString( const HtmlString &path, std::ios_base::openmode open_mode = std::ios::binary | std::ios::in );
		/// <summary>
		/// 写入文件
		/// </summary>
		/// <param name="path">文件路径</param>
		/// <param name="stringstream">文件内容</param>
		/// <param name="open_mode">打开模式</param>
		/// <param name="open_prot">属性</param>
		/// <returns>写入数量</returns>
		static size_t setFileAllWString( const std::string &path, const HtmlString &stringstream, std::ios_base::openmode open_mode = std::ios::binary | std::ios::out | std::ios::trunc );

		/// <summary>
		/// 读取文件
		/// </summary>
		/// <param name="path">文件路径</param>
		/// <param name="open_mode">打开模式</param>
		/// <param name="open_prot">属性</param>
		/// <returns>文件内容</returns>
		static HtmlString_Shared getFileAllWString( const std::string &path, std::ios_base::openmode open_mode = std::ios::binary | std::ios::in);
		
		/// <summary>
		/// 转码，从 HtmlString 转到 std::string
		/// </summary>
		/// <param name="txt">被转换的字符串</param>
		/// <param name="open_prot">转换完成的字符串</p
		static std::string converToStdString(const HtmlString& txt);
	};
}


#endif // HTMLDOCTOOLS_H_H_HEAD__FILE__
