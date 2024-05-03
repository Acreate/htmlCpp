#ifndef XDIR_H_H_HEAD__FILE__
#define XDIR_H_H_HEAD__FILE__
#pragma once

#include "../../nameSpace/cylHtmlTools.h"
namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT XDir {
	public:
		friend class XPath;
	private:
		std::list< HtmlString > namesList;
		HtmlStringMapToVectorHtmlStrSPtr attributesList;
		std::shared_ptr< std::unordered_map< HtmlString, Vector_XDirAttributeSPtr_Shared > > attributesMap;
	public:
		XDir( const cylHtmlTools::HtmlString &param );
		virtual ~XDir( );
	public: // 比较
		bool hasName( const HtmlString &dir_name ) const;
		bool hasAttribute( const HtmlStringPairUnorderMap_Shared &attribute, const HtmlString &nodeName );
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
			Replace // 替换
			, Add // 添加
			, Remove // 移除
			, Get // 获取
			, Error // 错误 - 定义的错误类型为 Error ~ Error + 4096
			, Try = Error + 4096 // 异常 - 定义的异常类型为 Try ~ Try + 4096
			, None = Try + 4096
		};
		/// <summary>
		/// 插入一个属性<br/>
		/// 操作状态: 
		/// </summary>
		/// <param name="dir_set_name">目录名称</param>
		/// <param name="xdir_set_type_name">属性名称</param>
		/// <param name="xDir_set_attribute_value">设置的属性值</param>
		/// <returns>返回操作状态</returns>
		XDir_Attribute_Status insertXDirAttributeInfo( const HtmlString &dir_set_name, const HtmlString &xdir_set_type_name, const HtmlString &xDir_set_attribute_value );
		/// <summary>
		/// 获取属性
		/// </summary>
		/// <param name="dir_set_name">目录名称</param>
		/// <param name="xdir_set_type_name">属性名称</param>
		/// <param name="xDir_result_attribute_value">返回的属性值</param>
		/// <returns>返回操作状态</returns>
		XDir_Attribute_Status tryAttributeGet( const HtmlString &dir_set_name, const HtmlString &xdir_set_type_name, Vector_HtmlStringSPtr &xDir_result_attribute_value );
	};
}

#endif // XDIR_H_H_HEAD__FILE__
