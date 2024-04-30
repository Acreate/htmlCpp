#ifndef HTMLTOOLS_H_H_HEAD__FILE__
#define HTMLTOOLS_H_H_HEAD__FILE__
#pragma once
#include <memory>
#include <memory>
#include <string>
#include <list>

#include "../../nameSpace/HtmlTools.h"
#include <ostream>

#include "../../wstr/WStrTools.h"

namespace htmlTools {
	class HTMLTOOLS_EXPORT XPath {
		Vector_XDirSPtr dirListSPtr; // 名称
		HtmlString separator; // 分隔符
	public:
		XPath( const HtmlString &wstr );
		XPath( const List_HtmlStringSptr &std_w_string_list_shared, const HtmlString &separator = L"/" );
		XPath( );
		virtual ~XPath( );
	private: // 指定 xpath
		/// <summary>
		/// 从根部访问的 xpath
		/// </summary>
		/// <param name="html_doc">节点文档</param>
		/// <returns>匹配列表</returns>
		Vector_HtmlNodeSPtr_Shared rootBuider( HtmlDoc_Shared html_doc );
		/// <summary>
		/// 从相对访问的 xpath <br/>
		/// 如果找不到则返回 nullptr
		/// </summary>
		/// <param name="html_node_shared">节点列表</param>
		/// <returns>匹配列表</returns>
		Vector_HtmlNodeSPtr_Shared relativeBuider( Vector_HtmlNodeSPtr html_node_shared );
		/// <summary>
		/// 从任意访问的 xpath <br/>
		/// 如果找不到则返回 nullptr
		/// </summary>
		/// <param name="html_node_shared">节点列表</param>
		/// <returns>匹配列表</returns>
		Vector_HtmlNodeSPtr_Shared anyBuider( Vector_HtmlNodeSPtr html_node_shared );
	public: // 随机 xpath
		/// <summary>
		/// 从列表节点当中使用 xpath 查找节点<br/>
		/// 找不到则返回 nullptr
		/// </summary>
		/// <param name="html_node_shared_s">查找的列表</param>
		/// <returns>节点列表</returns>
		Vector_HtmlNodeSPtr_Shared buider( Vector_HtmlNodeSPtr html_node_shared_s );
		/// <summary>
		/// 从文档对象当中使用 xpath 查找节点<br/>
		/// 找不到则返回 nullptr
		/// </summary>
		/// <param name="html_doc_shared">查找的列表</param>
		/// <returns>节点列表</returns>
		Vector_HtmlNodeSPtr_Shared buider( HtmlDoc_Shared html_doc_shared );
		inline operator HtmlString( ) const;
		inline HtmlString getHtmlString( ) const {
			return operator HtmlString( );
		}
	};

}

#endif // HTMLTOOLS_H_H_HEAD__FILE__
