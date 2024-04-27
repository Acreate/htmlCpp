#include "XPathTools.h"
HtmlTools::XPathTools::XPathTools( const std::wstring &wstr ) : XPathTools( ) {
	stdWStringListShared.emplace_back( std::make_shared< std::wstring >( wstr ) );
}
HtmlTools::XPathTools::XPathTools( ) {

}
