#ifndef HTMLTOOLS_H_H_HEAD__FILE__
#define HTMLTOOLS_H_H_HEAD__FILE__
#pragma once
#include <memory>
#include <memory>
#include <string>
#include <list>

#include "../../nameSpace/HtmlTools.h"
#include <ostream>

namespace HtmlTools {
	class HTMLTOOLS_EXPORT XPathTools {

		std::list< std::shared_ptr< std::wstring > > stdWStringListShared; // 名称
		std::wstring separator; // 分隔符
	public:
		XPathTools( const std::wstring &wstr );
		XPathTools( const std::list< std::shared_ptr< std::wstring > > &std_w_string_list_shared, const std::wstring &separator = L"/" ) : separator( separator ) {
			for( auto &stdWString : std_w_string_list_shared )
				stdWStringListShared.emplace_back( std::make_shared< std::wstring >( *stdWString ) );
		}
		XPathTools( );
		virtual ~XPathTools( ) { }
	public:
		std::wstring getWString( ) const {
			std::wstring resultl;
			for( auto stdWStrSPtr : stdWStringListShared )
				resultl.append( separator ).append( *stdWStrSPtr );
			return resultl;
		}
		std::list< std::wstring > getWStringPath( ) const {
			std::list< std::wstring > resultl;
			for( auto stdWStrSPtr : stdWStringListShared )
				resultl.emplace_back( *stdWStrSPtr );
			return resultl;
		}
	public:
		XPathTools & append( const std::wstring &wstr ) {
			stdWStringListShared.emplace_back( std::make_shared< std::wstring >( wstr ) );
			return *this;
		}
		XPathTools & operator <<( const std::wstring &wstr ) {
			stdWStringListShared.emplace_back( std::make_shared< std::wstring >( wstr ) );
			return *this;
		}
		XPathTools operator +( const std::wstring &wstr ) const {
			auto xPathTools = XPathTools( stdWStringListShared );
			xPathTools.stdWStringListShared.emplace_back( std::make_shared< std::wstring >( wstr ) );
			return xPathTools;
		}
		XPathTools operator -( const std::wstring &wstr ) const {
			auto xPathTools = XPathTools( );
			auto iterator = stdWStringListShared.begin( );
			auto end = stdWStringListShared.end( );
			for( ; iterator != end; ++iterator )
				if( *iterator->get( ) != wstr )
					xPathTools.stdWStringListShared.emplace_back( std::make_shared< std::wstring >( *iterator->get( ) ) );

			return xPathTools;
		}
		bool operator ==( const XPathTools &wstr ) const {
			if( this == &wstr )
				return true;
			if( wstr.stdWStringListShared.size( ) != stdWStringListShared.size( ) )
				return false;
			auto thisIter = this->stdWStringListShared.begin( );
			for( auto &wstrPtr : wstr.stdWStringListShared ) {
				if( *thisIter->get( ) != *wstrPtr.get( ) )
					return false;
				++thisIter;
			}
			return true;
		}
		bool operator ==( const std::wstring &wstr ) const {
			size_t wstrLen = wstr.length( );
			size_t size = this->stdWStringListShared.size( );
			if( wstrLen == 0 && size == 0 )
				return true;
			std::wstring buffWStr;
			auto iter = stdWStringListShared.begin( ), end = stdWStringListShared.end( );
			for( ; iter != end; ++iter ) {
				buffWStr.append( *iter->get( ) );
				if( buffWStr != wstr )
					return false;
			}
			return true;
		}
		friend bool operator ==( const std::wstring &wstr, const XPathTools &x_path_tools ) {
			return x_path_tools == wstr;
		}
		friend std::wostream & operator <<( std::wostream &os, const XPathTools &path_tools ) {
			return os << path_tools.getWString( );
		}
	};


}

#endif // HTMLTOOLS_H_H_HEAD__FILE__
