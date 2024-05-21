#ifndef XDIR_H_H_HEAD__FILE__
#define XDIR_H_H_HEAD__FILE__
#pragma once

#include "../../../nameSpace/cylHtmlTools.h"
namespace cylHtmlTools {
	/// <summary>
	/// XPtah 属性
	/// </summary>
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
		/// <summary>
		/// 使用 xptah 中的路径节点当做参数解析
		/// </summary>
		/// <param name="param">适配 xpath 当中的路径节点</param>
		XDir( const cylHtmlTools::HtmlString &param );
		/// <summary>
		/// 使用名称与 XDirAttribute 实现初始化
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="xdiattrbute_sptr">XDirAttribute 对象指针</param>
		XDir( const cylHtmlTools::HtmlString &name, const XDirAttribute_Shared &xdiattrbute_sptr );

		/// <summary>
		/// 使用名称与 XDirAttribute 实现初始化
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="xdiattrbute">XDirAttribute 对象</param>
		XDir( const cylHtmlTools::HtmlString &name, const XDirAttribute &xdiattrbute );
		/// <summary>
		/// 使用名称与 XDirAttribute 实现初始化
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="vector_xdiattrbutesptr_sptr">XDirAttribute 对象指针列表</param>
		XDir( const cylHtmlTools::HtmlString &name, const Vector_XDirAttributeSPtr_Shared &vector_xdiattrbutesptr_sptr );

		/// <summary>
		/// 使用名称与 XDirAttribute 实现初始化
		/// </summary>
		/// <param name="name">名称</param>
		/// <param name="vector_xdiattrbutesptr">XDirAttribute 对象指针列表</param>
		XDir( const cylHtmlTools::HtmlString &name, const Vector_XDirAttributeSPtr &vector_xdiattrbutesptr );

		virtual ~XDir( );
	public: // 比较
		bool hasName( const HtmlString &dir_name ) const;
		/// <summary>
		/// 查询当前目录属性是否在映射列表当中找到存在匹配
		/// </summary>
		/// <param name="attribute">属性映射列表</param>
		/// <param name="nodeName">节点名称</param>
		/// <returns>存在匹配返回 true</returns>
		bool hasAttribute( const UMap_HtmlStringK_HtmlStringV_Shared &attribute, const HtmlString &nodeName ) const;
	public: // 属性
		UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV getAttributeMaps( ) {
			return *attributesMap;
		}
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
		/// <summary>
		/// 比较两个列表，检测左侧列表是否包含右侧列表，包含返回 true，否则返回 false
		/// </summary>
		/// <param name="left_xdirattributeSPtr_vector">左操作符列表</param>
		/// <param name="right_xdirattributeSPtr_vector">右操作符列表</param>
		/// <returns>包含返回 true，否则返回 false</returns>
		static bool isLeftXDirIncludeRightXDir( const Vector_XDirAttributeSPtr &left_xdirattributeSPtr_vector, Vector_XDirAttributeSPtr &right_xdirattributeSPtr_vector );
		/// <summary>
		/// 比较左侧是否包含右侧
		/// </summary>
		/// <param name="left_xdir">超集</param>
		/// <param name="right_xdir">子集</param>
		/// <returns>true 表示 right_xdir 是 left_xdir 子集</returns>
		static bool isLeftXDirIncludeRightXDir( const XDir *left_xdir, const XDir *right_xdir );
		/// <summary>
		/// 比较左侧是否包含右侧
		/// </summary>
		/// <param name="left_xdir">超集</param>
		/// <param name="right_xdir">子集</param>
		/// <returns>true 表示 right_xdir 是 left_xdir 子集</returns>
		static bool isLeftXDirIncludeRightXDir( const XDir &left_xdir, const XDir &right_xdir ) {
			return isLeftXDirIncludeRightXDir( &left_xdir, &right_xdir );
		}
		/// <summary>
		/// oter_xdir_sptr 是否包含 本对象 this ⊆ oter_xdir_sptr<br/>
		/// this 是否是 oter_xdir_sptr 子集
		/// </summary>
		/// <param name="oter_xdir_sptr">检测超集</param>
		/// <returns>该对象为子集，则返回 true</returns>
		bool isOtherXDirIncludeThisXDir( const XDir_Shared &oter_xdir_sptr ) const {
			return isLeftXDirIncludeRightXDir( oter_xdir_sptr.get( ), this );
		}
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
		/// <param name="dir_set_name"></param>
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
