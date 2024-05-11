#ifndef XDIR_H_H_HEAD__FILE__
#define XDIR_H_H_HEAD__FILE__
#pragma once

#include "../../../nameSpace/cylHtmlTools.h"
namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT XDir {
	public:
		friend class XPath;
	private:
		/// <summary>
		/// 超目录的名称
		/// </summary>
		std::list< HtmlString > namesList;
		/// <summary>
		/// 超目录文件名与超目录信息的映射
		/// </summary>
		UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV_Shared attributesMap;
	public:
		XDir( const cylHtmlTools::HtmlString &param );
		virtual ~XDir( );
	public: // 比较
		bool hasName( const HtmlString &dir_name ) const;
		/// <summary>
		/// 查询当前目录属性是否在映射列表当中找到存在匹配
		/// </summary>
		/// <param name="attribute">属性映射列表</param>
		/// <param name="nodeName">节点名称</param>
		/// <returns>存在匹配返回 true</returns>
		bool hasAttribute( const UMap_HtmlStringK_HtmlStringV_Shared &attribute, const HtmlString &nodeName );
	public: // 属性

		/// <summary>
		/// 获取名称
		/// </summary>
		/// <returns></returns>
		HtmlString getDirName( ) const;
		/// <summary>
		/// 包含属性的名称
		/// </summary>
		/// <returns>具备 [] 属性的名称</returns>
		HtmlString getXDirName( ) const;
	public: // - 属性映射z
		enum XDir_Attribute_Status {
			Replace_Element // 替换元素
			, Add_Element // 添加元素
			, Append_Element // 追加元素
			, Add_Array // 添加数组
			, Remove_Element // 移除元素
			, Remove_Array // 移除数组
			, Get_Element // 获取元素
			, Get_Array // 获取数组
			, Error_Element // 元素错误 - 定义的错误类型为 Error ~ Error + 4096
			, Try_Array = Error_Element + 4096 // 组异常 - 定义的异常类型为 Try ~ Try + 4096
			, None = Try_Array + 4096
		};
		/// <summary>
		/// 插入一个属性<br/>
		/// 操作状态: 
		/// </summary>
		/// <param name="dir_set_name">目录名称</param>
		/// <param name="xdir_info">属性</param>
		/// <returns>返回操作状态</returns>
		XDir_Attribute_Status insertXDirAttributeInfo( const HtmlString &dir_set_name, XDirAttribute_Shared &xdir_info );
		/// <summary>
		/// 获取属性
		/// </summary>
		/// <param name="dir_set_name">目录名称</param>
		/// <param name="xDir_result_attribute_value">返回的属性值</param>
		/// <returns>返回操作状态</returns>
		XDir_Attribute_Status tryAttributeGet( const HtmlString &dir_set_name, Vector_XDirAttributeSPtr &xDir_result_attribute_value ) const;
	};
}

#endif // XDIR_H_H_HEAD__FILE__
