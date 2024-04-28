#ifndef HTMLTOOLS_H_H_HEAD__FILE__
#define HTMLTOOLS_H_H_HEAD__FILE__
#pragma once
#include <memory>
#include <memory>
#include <string>
#include <list>

#include "../../nameSpace/HtmlTools.h"
#include <ostream>

namespace htmlTools {
	class HTMLTOOLS_EXPORT XPath {
		List_HtmlStringSptr stdWStringListShared; // 名称
		std::wstring separator; // 分隔符
	public:
		XPath( const HtmlString &wstr );
		XPath( const List_HtmlStringSptr &std_w_string_list_shared, const HtmlString &separator = L"/" );
		XPath( );
		virtual ~XPath( );
	public:
		Vector_HtmlNodeSPtr_Shared buider( HtmlNode_Shared html_node_shared );
		operator HtmlString( ) const {
			HtmlString result;
			for( auto &str : stdWStringListShared )
				result = result.append( *str ) + charValue::forwardSlash;
			return result;
		}
		HtmlString getHtmlString( ) const {
			return operator HtmlString( );
		}
	};

}

#endif // HTMLTOOLS_H_H_HEAD__FILE__
