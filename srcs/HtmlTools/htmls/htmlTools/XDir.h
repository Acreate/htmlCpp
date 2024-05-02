#ifndef XDIR_H_H_HEAD__FILE__
#define XDIR_H_H_HEAD__FILE__
#pragma once

#include "../../nameSpace/HtmlTools.h"
namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT XDir {
	public:
		friend class XPath;
	private:
		std::list< HtmlString > namesList;
		HtmlStringMapToVectorHtmlStrSPtr attributesList;
		std::shared_ptr< std::unordered_map< HtmlString, HtmlStringPairUnorderMap > > attributesMap;
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
	};
}

#endif // XDIR_H_H_HEAD__FILE__
