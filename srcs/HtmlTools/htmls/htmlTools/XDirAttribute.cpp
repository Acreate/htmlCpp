#include "XDirAttribute.h"

#include "../../htmlString/HtmlStringTools.h"
using namespace cylHtmlTools;
XDirAttribute::XDirAttribute( const HtmlString_Shared &in_name, const Vector_HtmlStringSPtr_Shared &in_values ) {
	name = in_name;
	values = in_values;
}

XDirAttribute::XDirAttribute( const HtmlChar *name_buff, const size_t name_buff_size, const HtmlChar *values_buff, const size_t values_size ) {
	name = converXDirAttributeName( name_buff, name_buff_size );
	values = converXDirAttributeValues( values_buff, values_size );
}
XDirAttribute::~XDirAttribute( ) {
}
// todo : 实现属性
HtmlString_Shared XDirAttribute::converXDirAttributeName( const HtmlChar *buff, const size_t buff_size ) {
	return std::make_shared< HtmlString >( buff, buff_size );
}
// todo : 实现值
Vector_HtmlStringSPtr_Shared XDirAttribute::converXDirAttributeValues( const HtmlChar *buff, const size_t buff_size ) {
	if( HtmlStringTools::equRemoveSpaceOverHtmlString( *name, HtmlString( L"class" ) ) ) {
		
	}
	return { };
}
