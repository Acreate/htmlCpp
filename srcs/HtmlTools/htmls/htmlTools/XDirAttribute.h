#ifndef XDIRATTRIBUTE_H_H_HEAD__FILE__
#define XDIRATTRIBUTE_H_H_HEAD__FILE__
#pragma once
#include "../../nameSpace/cylHtmlTools.h"
namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT XDirAttribute {
	public:
		friend class XDir;
	private:
		HtmlString_Shared name;
		Vector_HtmlStringSPtr_Shared values;
	public:
		XDirAttribute( const HtmlString_Shared &in_name, const Vector_HtmlStringSPtr_Shared &in_values );
		XDirAttribute( const HtmlChar *name_buff, const size_t name_buff_size, const HtmlChar *values_buff, const size_t values_size );
		virtual ~XDirAttribute( );
	public: // - 属性
		HtmlString_Shared getName( ) const {
			return name;
		}
		Vector_HtmlStringSPtr_Shared getValues( ) const {
			return values;
		}
	private: // - 解析函数
		/// <summary>
		/// 从缓冲当中转换到名称
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <returns>名称对象指针，失败返回 nullptr</returns>
		HtmlString_Shared converXDirAttributeName( const HtmlChar *buff, const size_t buff_size );
		/// <summary>
		/// 从缓冲当中转换到值
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <returns>值列表对象指针，失败返回 nullptr</returns>
		Vector_HtmlStringSPtr_Shared converXDirAttributeValues( const HtmlChar *buff, const size_t buff_size );
	};
}


#endif // XDIRATTRIBUTE_H_H_HEAD__FILE__
