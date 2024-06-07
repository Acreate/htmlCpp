#ifndef HTMLNODE_H_H_HEAD__FILE__
#define HTMLNODE_H_H_HEAD__FILE__
#pragma once
#include <functional>
#include <string>
#include <unordered_map>

#include "../../htmlString/HtmlStringTools.h"
#include "../../nameSpace/cylHtmlTools.h"

#include "../enum/HtmlNodeType/Html_Node_Type.h"
#include "../htmlDoc/HtmlDoc.h"

namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT HtmlNode {
	public:
		friend class HtmlDoc;
		friend class XPath;
		friend class HtmlDocTools;
	private: // 私有成员变量
		/// <summary>
		/// 字符串指针
		/// </summary>
		HtmlString_Shared czWStr = nullptr;
		/// <summary> 
		/// 开始节点，仅 nodeType 等于 Html_Node_Type::DoubleNode 有效
		/// </summary>
		HtmlNode_Shared startNode = nullptr;
		/// <summary>
		/// 结束节点，仅 nodeType 等于 Html_Node_Type::DoubleNode 有效
		/// </summary>
		HtmlNode_Shared endNode = nullptr;
		/// <summary>
		/// 字符串偏移量
		/// </summary>
		size_t ptrOffset = 0;
		/// <summary>
		/// 字符串长度
		/// </summary>
		size_t ptrCWtrLen = 0;
		/// <summary>
		/// 节点类型
		/// </summary>
		Html_Node_Type nodeType = Html_Node_Type::None;
	private:
		HtmlDoc_Shared htmldocShared;
		HtmlNode_Shared parent; // 父节点
		Vector_HtmlNodeSPtr_Shared brother; // 友邻(同级/兄弟)节点
		Vector_HtmlNodeSPtr_Shared subChildren; // 子节点
		HtmlNode_Shared thisSharedPtr; // 当前节点
		UMap_HtmlStringK_HtmlStringV_Shared refNodeAttributes; // 当前节点的所有属性
	private:
		static void setParent( const HtmlNode_Shared &child, const HtmlNode_Shared &parent );
	public: // - 属性节点
		void setParent( const HtmlNode_Shared &parent ) {
			setParent( thisSharedPtr, parent );
		}
		HtmlNode_Shared getParent( ) const {
			return parent;
		}
		Vector_HtmlNodeSPtr_Shared getChildren( ) const {
			return subChildren;
		}
		Vector_HtmlNodeSPtr_Shared getBrother( ) const {
			return brother;
		}
	public:
		HtmlNode( );
		virtual ~HtmlNode( );
	public:
		/// <summary>
		/// 解析属性<br/>
		/// 键值对配置的属性列表
		/// </summary>
		/// <returns>属性映射表</returns>
		UMap_HtmlStringK_HtmlStringV_Shared analysisAttribute( );
		/// <summary>
		/// 获取整个节点
		/// </summary>
		HtmlString_Shared getNodeContent( ) const;
		/// <summary>
		/// 获取节点名称
		/// </summary>
		HtmlString_Shared getNodeName( ) const;
		/// <summary>
		/// 获取节点类型
		/// </summary>
		Html_Node_Type getNodeType( ) const;
		/// <summary>
		/// 如果是双节点，那么返回开始节点
		/// </summary>
		HtmlNode_Shared getStartNode( ) const;
		/// <summary>
		/// 如果是双节点，那么返回结束节点
		/// </summary>
		HtmlNode_Shared getEndNode( ) const;
		/// <summary>
		/// 获取节点大小
		/// </summary>
		size_t nodeSize( ) const;
		/// <summary>
		/// 获取节点基于字符串的偏移
		/// </summary>
		size_t getPtrOffset( ) const;
		/// <summary>
		/// 获取节点的字符串长度<br/>
		/// 它只会返回基于 < 到 > 的长度。并不会返回双节点中的结束节点之间的内容
		/// </summary>
		size_t getPtrCWtrLen( ) const;
		/// <summary>
		/// 获取的内容-节点本色与包含内容（未转换）
		/// </summary>
		HtmlString_Shared getIncludeNodeContent( ) const;
		/// <summary>
		/// 获取节点的路径
		/// </summary>
		HtmlString_Shared getPath( ) const;
		/// <summary>
		/// 获取节点当中文本内容<br/>
		/// 节点之间的内容会相对应的转换
		/// </summary>
		HtmlString_Shared getNodeIncludeContentText( ) const;

		/// <summary>
		/// 查找节点属性<br/>
		/// 查找过程需要调用校验函数，当校验函数返回true时，则判断该节点可命中<br/>
		/// 命中时，可携返回
		/// </summary>
		/// <param name="callFunction">校验函数</param>
		/// <returns>命中列表</returns>
		UMap_HtmlStringK_HtmlStringV_Shared findAttribute( const std::function< bool( const HtmlString &attributeName, const HtmlString &attributeValue ) > &callFunction ) const;
		/// <summary>
		/// 使用 xpath 查找元素
		/// </summary>
		/// <param name="xpath">xpath</param>
		/// <returns>节点列表</returns>
		Vector_HtmlNodeSPtr_Shared xpath( const HtmlString &xpath );

		/// <summary>
		/// 获取所有根节点
		/// </summary>
		/// <returns>根节点列表</returns>
		Vector_HtmlNodeSPtr_Shared getHtmlNodeRoots( );
		/// <summary>
		/// 节点转换到 XDir 对象指针
		/// </summary>
		/// <returns>失败返回 nullptr</returns>
		XDir_Shared converXDirSptr( ) {
			return htmldocShared->converXDirSptr( thisSharedPtr );
		}
	public: // - 校验节点
		/// <summary>
		/// 是否是开始节点
		/// </summary>
		/// <returns>开始节点为 true</returns>
		 bool isStartNode( ) const;
		/// <summary>
		/// 是否是结束节点
		/// </summary>
		/// <returns>结束节点为true</returns>
		 bool isEndNode( ) const;
		/// <summary>
		/// 是否是双节点
		/// </summary>
		/// <returns>双节点返回 true</returns>
		 bool isDoubleNodeType( ) const;
	private:
		/// <summary>
		/// 生成 < 与 > 的配对
		/// </summary>
		/// <param name="html_doc_shared">html 文档对象指针</param>
		/// <param name="start_index">遍历的开始下标</param>
		/// <param name="max_index">遍历的结束下标</param>
		/// <param name="index_count">遍历的个数</param>
		/// <returns>配对列表</returns>
		static Vector_HtmlNodeSPtr_Shared parseHtmlNodeCharPair( const HtmlDoc_Shared &html_doc_shared, size_t start_index, const size_t max_index, size_t &index_count );
	public: // 比较
		bool operator==( const HtmlNode &rightNode ) const {
			if( this == &rightNode || thisSharedPtr == rightNode.thisSharedPtr )
				return true;
			if( isEndNode( ) )
				return *this->startNode.get( ) == rightNode;
			if( rightNode.isEndNode( ) )
				return *this == *rightNode.startNode;
			if( ptrOffset == rightNode.ptrOffset && ptrCWtrLen == rightNode.ptrCWtrLen && htmldocShared == rightNode.htmldocShared && *czWStr == *rightNode.czWStr )
				return true;
			return false;
		}

		bool operator!=( const HtmlNode &right ) const {
			return !( this->operator==( right ) );
		}
	};

}

#endif // HTMLNODE_H_H_HEAD__FILE__
