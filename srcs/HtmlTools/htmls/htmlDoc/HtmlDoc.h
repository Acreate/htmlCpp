#ifndef HTMLDOC_H_H_HEAD__FILE__
#define HTMLDOC_H_H_HEAD__FILE__
#pragma once
#include <functional>
#include <memory>
#include <string>

#include "../../nameSpace/HtmlTools.h"

#include "../enum/HtmlNodeType/Html_Node_Type.h"

namespace HtmlTools {
	class HTMLTOOLS_EXPORT HtmlDoc {
	public: // 友元
		friend class HtmlNode;
	private:
		/// <summary>
		/// 查找下一个节点结束符的位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeEndChar( const std::shared_ptr< std::wstring > std_c_w_string, size_t &max_index, size_t &start_index );

		/// <summary>
		/// 查找下一个节点开始符的位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeStartChar( const std::shared_ptr< std::wstring > std_c_w_string, size_t &max_index, size_t &start_index );

		/// <summary>
		/// 查找下一个节点的斜杠位置
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="max_index">字符串的推测长度</param>
		/// <param name="start_index">遍历下标</param>
		/// <returns>true 表示存在</returns>
		static bool findNextNodeForwardSlash( const std::shared_ptr< std::wstring > std_c_w_string, size_t &max_index, size_t &start_index );

		/// <summary>
		/// 判断节点是否为单节点
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向节点开始字符 '<'，如果不是，那么它将会向后查询</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示单下标</returns>
		static bool isSingelNode( const std::shared_ptr< std::wstring > std_c_w_string, size_t &start_index, size_t &end_index );

		/// <summary>
		/// 判断节点是否为开始节点
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向 '<'</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示单下标</returns>
		static bool isStartNode( const std::shared_ptr< std::wstring > std_c_w_string, size_t &start_index, size_t &end_index );

		/// <summary>
		/// 判断节点是否为结束节点
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向 '<'</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示单下标</returns>
		static bool isEndNode( const std::shared_ptr< std::wstring > std_c_w_string, size_t &start_index, size_t &end_index );

		/// <summary>
		/// 判断节点是否为注释节点(包含 !DOCTYPE 节点)
		/// </summary>
		/// <param name="std_c_w_string">查找字符串</param>
		/// <param name="start_index">遍历下标，始终指向节点开始字符 '<'，如果不是，那么它将会向后查询</param>
		/// <param name="end_index">结束下标，始终指向节点结束字符 '>'</param>
		/// <returns>true 表示注释节点</returns>
		static bool isAnnotation( const std::shared_ptr< std::wstring > std_c_w_string, size_t &start_index, size_t &end_index );

		/// <summary>
		/// 解析双节点，匹配第一个指向的节点。
		/// </summary>
		/// <param name="html_doc_shared">引用节点</param>
		/// <param name="html_node">匹配的双节点</param>
		/// <param name="html_node_char_pairs">节点列表</param>
		/// <param name="start_index">开始的节点列表下标</param>
		/// <param name="end_index">结束的节点列表下标</param>
		/// <returns>匹配的节点列表</returns>
		static Vector_HtmlNodeSPtr_Shared analysisDoubleNode( HtmlDoc_Shared html_doc_shared, HtmlNode_Shared html_node, Vector_HtmlNodeSPtr_Shared html_node_char_pairs, size_t &start_index, size_t &end_index );
	public: // 静态对象生成器
		/// <summary>
		/// 根据字符串内容生成节点列表
		/// </summary>
		/// <param name="std_c_w_string">指向字符串的指针</param>
		/// <param name="end_index">结束下标</param>
		/// <param name="start_index">开始下标，最终下标</param>
		/// <returns>解析后的列表</returns>
		static HtmlDoc_Shared parse( const std::shared_ptr< std::wstring > std_c_w_string, size_t &end_index, size_t &start_index );
	private: // 核心成员
		/// <summary>
		/// 引用的 html 内容
		/// </summary>
		std::shared_ptr< std::wstring > htmlWCStr;

		/// <summary>
		/// 文件的顶级节点
		/// </summary>
		Vector_HtmlNodeSPtr_Shared htmlDocNode;
	public:
		/// <summary>
		/// 获取指定名称的节点
		/// </summary>
		/// <param name="nodeName">获取的名称</param>
		/// <returns>节点，失败返回 nullptr</returns>
		HtmlNode_Shared getNodeFromName( const std::wstring &nodeName ) const;

		/// <summary>
		/// 获取指定的节点<br/>
		/// 未发生 return true 时，该函数会继续滴啊用 callFun 遍历成员
		/// </summary>
		/// <param name="callFun">校验函数</param>
		/// <returns>节点，失败返回 nullptr</returns>
		HtmlNode_Shared getNodeFromName( const std::function< bool( const std::wstring &, Html_Node_Type ) > &callFun ) const;
		/// <summary>
		/// 遍历获取节点<br/>
		/// 节点传递到 callFun 调用参数当中，当需要存储时，返回 true。直到结束
		/// </summary>
		/// <param name="callFun">校验函数</param>
		/// <returns>返回列表</returns>
		Vector_HtmlNodeSPtr_Shared getNodes( const std::function< bool( const HtmlNode_Shared & ) > &callFun );
	private: // 无法使用个构造函数
		HtmlDoc( );
	public:
		virtual ~HtmlDoc( );
	public: // 功能转换

		/// <summary>
		/// 本对象关联的节点解析兄弟关系
		/// </summary>
		/// <returns>解析完成的节点列表</returns>
		Vector_HtmlNodeSPtr_Shared analysisBrotherNode( );
		/// <summary>
		/// 解析属性
		/// </summary>
		/// <returns>解析完毕的列表</returns>
		Vector_HtmlNodeSPtr_Shared analysisAttributesNode( );
	public: // 节点获取属性

		/// <summary>
		/// 获取整个节点
		/// </summary>
		std::shared_ptr< std::wstring > getWSNode( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 获取节点名称
		/// </summary>
		std::shared_ptr< std::wstring > getNodeWSName( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 获取节点类型
		/// </summary>
		Html_Node_Type getNodeType( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 如果是双节点，那么返回开始节点
		/// </summary>
		std::shared_ptr< HtmlNode > getStartNode( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 如果是双节点，那么返回结束节点
		/// </summary>
		std::shared_ptr< HtmlNode > getEndNode( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 获取节点大小
		/// </summary>
		size_t nodeSize( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 获取节点基于字符串的偏移
		/// </summary>
		size_t getPtrOffset( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 获取节点的字符串长度<br/>
		/// 它只会返回基于 < 到 > 的长度。并不会返回双节点中的结束节点之间的内容
		/// </summary>
		size_t getPtrCWtrLen( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 获取的内容
		/// </summary>
		StdWString_Shared getContent( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 获取节点的路径
		/// </summary>
		StdWString_Shared getPath( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 获取节点当中文本内容<br/>
		/// 节点之间的内容会相对应的转换
		/// </summary>
		StdWString_Shared getNodeText( const HtmlNode_Shared node_shared ) const;
		/// <summary>
		/// 查找节点属性<br/>
		/// 查找过程需要调用校验函数，当校验函数返回true时，则判断该节点可命中<br/>
		/// 命中时，可携返回
		/// <param name="node_shared">查找的节点</param>
		/// <param name="callFunction">校验函数</param>
		/// <returns>命中列表</returns>
		bool findAttribute( const HtmlNode_Shared node_shared, const std::function< bool( const WStringPairUnorderMap_Shared ) > callFunction ) const;
		/// <summary>
		/// 查找子节点<br/>
		/// 查找过程需要调用校验函数，当校验函数返回true时，则判断该节点可命中<br/>
		/// 命中时，可携返回
		/// </summary>
		/// <param name="node_shared">查找的节点</param>
		/// <param name="callFunction">校验函数</param>
		/// <returns>命中列表</returns>
		Vector_HtmlNodeSPtr_Shared matchChildrenNodes( const HtmlNode_Shared node_shared, const std::function< bool( const HtmlNode_Shared ) > callFunction );
	};
}

#endif // HTMLDOC_H_H_HEAD__FILE__
