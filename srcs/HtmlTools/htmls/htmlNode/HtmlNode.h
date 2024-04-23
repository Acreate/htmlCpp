﻿#ifndef HTMLNODE_H_H_HEAD__FILE__
#define HTMLNODE_H_H_HEAD__FILE__
#pragma once
#include <string>
#include <unordered_map>

#include "../../nameSpace/HtmlTools.h"

#include "../enum/HtmlNodeType/Html_Node_Type.h"

namespace HtmlTools {
	class HTMLTOOLS_EXPORT HtmlNode {
	public:
		friend class HtmlDoc;
	private: // 私有成员变量
		/// <summary>
		/// 字符串指针
		/// </summary>
		std::shared_ptr< std::wstring > czWStr = nullptr;
		/// <summary> 
		/// 开始节点，仅 nodeType 等于 Html_Node_Type::DoubleNode 有效
		/// </summary>
		std::shared_ptr< HtmlNode > startNode = nullptr;
		/// <summary>
		/// 结束节点，仅 nodeType 等于 Html_Node_Type::DoubleNode 有效
		/// </summary>
		std::shared_ptr< HtmlNode > endNode = nullptr;
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
		HtmlNode_Shared parent; // 父节点
		Vector_HtmlNodeSPtr_Shared brother; // 友邻(同级/兄弟)节点
		Vector_HtmlNodeSPtr_Shared subChildren; // 子节点
		HtmlNode_Shared thisSharedPtr; // 当前节点
		WStringPairUnorderMap_Shared refNodeAttributes; // 当前节点的所有属性
	private:
		static void setParent( HtmlNode_Shared child, HtmlNode_Shared parent );
	public:
		void setParent( HtmlNode_Shared parent ) const {
			setParent( thisSharedPtr, parent );
		}
	public:
		HtmlNode( );
		virtual ~HtmlNode( );
	private:
		/// <summary>
		/// 解析属性<br/>
		/// 键值对配置的属性列表
		/// </summary>
		/// <returns>属性映射表</returns>
		WStringPairUnorderMap_Shared analysisAttribute( );
	private:
		/// <summary>
		/// 生成 < 与 > 的配对
		/// </summary>
		/// <param name="std_w_str">检查的字符串</param>
		/// <param name="start_index">遍历的开始下标</param>
		/// <param name="max_index">遍历的结束下标</param>
		/// <param name="index_count">遍历的个数</param>
		/// <returns>配对列表</returns>
		static Vector_HtmlNodeSPtr_Shared parseHtmlNodeCharPair( std::shared_ptr< std::wstring > std_w_str, size_t start_index, const size_t max_index, size_t &index_count );
	};

}

#endif // HTMLNODE_H_H_HEAD__FILE__