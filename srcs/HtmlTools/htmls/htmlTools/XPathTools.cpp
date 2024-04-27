#include "XPathTools.h"
HtmlTools::XPathTools::XPathTools( const std::wstring &wstr ) : XPathTools( ) {
	*stdWStringShared = wstr;
}
HtmlTools::XPathTools::XPathTools( ) : stdWStringShared( new std::wstring ) {

}
