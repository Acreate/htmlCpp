#ifndef HTMLTOOLS_H_H_HEAD__FILE__
#define HTMLTOOLS_H_H_HEAD__FILE__
#pragma once
#include <memory>
#include <memory>
#include <string>
#include <list>

#include "../../nameSpace/cylHtmlTools.h"
#include <ostream>

#include "../../htmlString/HtmlStringTools.h"

namespace cylHtmlTools {
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
		Vector_HtmlNodeSPtr_Shared rootBuider( HtmlDoc_Shared &html_doc );
		/// <summary>
		/// 从相对访问的 xpath <br/>
		/// 如果找不到则返回 nullptr
		/// </summary>
		/// <param name="html_node_shared">节点列表</param>
		/// <returns>匹配列表</returns>
		Vector_HtmlNodeSPtr_Shared relativeBuider( HtmlNode_Shared &html_node_shared );
		/// <summary>
		/// 从任意访问的 xpath <br/>
		/// 如果找不到则返回 nullptr
		/// </summary>
		/// <param name="html_node_shared">节点列表</param>
		/// <returns>匹配列表</returns>
		Vector_HtmlNodeSPtr_Shared anyBuider( Vector_HtmlNodeSPtr &html_node_shared );
	public: // 随机 xpath
		/// <summary>
		/// 从列表节点当中使用 xpath 查找节点<br/>
		/// 找不到则返回 nullptr
		/// </summary>
		/// <param name="html_node_shared_s">查找的列表</param>
		/// <returns>节点列表</returns>
		Vector_HtmlNodeSPtr_Shared buider( Vector_HtmlNodeSPtr_Shared &html_node_shared_s );
		/// <summary>
		/// 从文档对象当中使用 xpath 查找节点<br/>
		/// 找不到则返回 nullptr
		/// </summary>
		/// <param name="html_doc_shared">查找的列表</param>
		/// <returns>节点列表</returns>
		Vector_HtmlNodeSPtr_Shared buider( HtmlDoc_Shared &html_doc_shared );
		/// <summary>
		/// 使用的 路径控制器属性
		/// </summary>
		enum XDir_Control_Type {
			Cd_None // 不存在
			, Cd_Parent // 上级
			, Cd_Current // 当前-兄弟
			, Cd_Root // 根节点
		};
		/// <summary>
		/// 目录控制符
		/// </summary>
		/// <param name="current_find_nodes">控制节点列表</param>
		/// <param name="xdirInfo">目录信息</param>
		/// <param name="current_control_type">控制符类型</param>
		/// <param name="old_control_type">上次控制符类型</param>
		/// <returns>经过控制符转换的节点</returns>
		Vector_HtmlNodeSPtr pathControlDirName( Vector_HtmlNodeSPtr &current_find_nodes, XDir *xdirInfo, XDir_Control_Type current_control_type, XDir_Control_Type old_control_type );

		/// <summary>
		/// 匹配全部节点<br/>
		/// 从 currentFindNodes 获取 HtmlDoc 引用，得到位于该 HtmlDoc 引用下的全部节点，再运行匹配机制
		/// </summary>
		/// <param name="currentFindNodes">HtmlDoc获取的引用</param>
		/// <param name="x_dir">目录属性</param>
		/// <param name="path">历史目录(不包含属性)</param>
		/// <returns>查找到的列表</returns>
		Vector_HtmlNodeSPtr matchesHtmlDocAllNodes( Vector_HtmlNodeSPtr &currentFindNodes, XDir *x_dir, HtmlString &path );
	public: // - 类型转换
		operator HtmlString( ) const;
		/// <summary>
		/// 获取字符串系列(包含属性)
		/// </summary>
		inline HtmlString getHtmlString( ) const {
			return operator HtmlString( );
		}
	};

}

#endif // HTMLTOOLS_H_H_HEAD__FILE__
