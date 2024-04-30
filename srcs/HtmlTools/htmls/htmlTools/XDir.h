#ifndef XDIR_H_H_HEAD__FILE__
#define XDIR_H_H_HEAD__FILE__
#pragma once

#include "../../nameSpace/HtmlTools.h"
namespace htmlTools {
	class HTMLTOOLS_EXPORT XDir {
	public:
		friend class XPath;
	private:
		std::list< HtmlString > namesList;
		HtmlStringMapToVectorHtmlStrSPtr attributesList;
		std::unordered_map< HtmlString, HtmlStringPairUnorderMap > attributesMap;
	public:
		XDir( const htmlTools::HtmlString &param );
		virtual ~XDir( );
	public: // 比较
		bool hasName( const HtmlString &dir_name );
		bool hasAttribute( const HtmlStringPairUnorderMap_Shared &attribute,const HtmlString &nodeName );
	public: // 属性
		HtmlString getDirName( );
	};
}

#endif // XDIR_H_H_HEAD__FILE__
