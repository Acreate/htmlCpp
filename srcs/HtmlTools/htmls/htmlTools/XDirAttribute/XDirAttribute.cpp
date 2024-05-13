#include "XDirAttribute.h"

#include "../../../htmlString/HtmlStringTools.h"
using namespace cylHtmlTools;
XDirAttribute::XDirAttribute( const HtmlString_Shared &in_name, const Vector_HtmlStringSPtr_Shared &in_values ) {
	name = in_name;
	if( in_values )
		values = in_values;
	else
		values = std::make_shared< Vector_HtmlStringSPtr >( );
}
XDirAttribute::XDirAttribute( const HtmlString &in_name, const Vector_HtmlStringSPtr &in_values ): XDirAttribute( std::make_shared< HtmlString >( in_name ), std::make_shared< Vector_HtmlStringSPtr >( in_values ) ) {

}
XDirAttribute::XDirAttribute( const HtmlString &in_name, const std::vector< HtmlString > &in_values ) {
	name = std::make_shared< HtmlString >( in_name );
	values = std::make_shared< Vector_HtmlStringSPtr >( );
	for( auto &value : in_values )
		values->emplace_back( std::make_shared< HtmlString >( value ) );
}

XDirAttribute::XDirAttribute( const HtmlChar *name_buff, const size_t name_buff_size, const HtmlChar *values_buff, const size_t values_size ) {
	name = converXDirAttributeName( name_buff, name_buff_size );
	values = converXDirAttributeValues( values_buff, values_size, name );
}
XDirAttribute::~XDirAttribute( ) {
}

HtmlString XDirAttribute::serializeToHtmlString( ) const {
	HtmlString result( 1, charValue::at );
	result.append( *name ).append( HtmlString( 1, charValue::equ ) );
	auto iterator = values->begin( );
	auto end = values->end( );
	if( iterator == end )
		return result;
	HtmlString doubleQuoation( 1, charValue::doubleQuotation );
	HtmlString space( 1, charValue::space );
	result.append( doubleQuoation );
	do {
		result.append( *iterator->get( ) );
		if( iterator == end )
			break;
		result.append( space );
		++iterator;
	} while( true );

	result.append( doubleQuoation );
	return result;
}

HtmlString_Shared XDirAttribute::converXDirAttributeName( const HtmlChar *buff, const size_t buff_size ) {
	return std::make_shared< HtmlString >( buff, buff_size );
}

Vector_HtmlStringSPtr_Shared XDirAttribute::converXDirAttributeValues( const HtmlChar *buff, const size_t buff_size, HtmlString_Shared &name ) {
	auto resultVectorSPtr = std::make_shared< Vector_HtmlStringSPtr >( );
	if( HtmlStringTools::equRemoveSpaceOverHtmlString( *name, HtmlString( L"class" ) ) ) {
		size_t get_quoation_position_end;
		std::vector< std::pair< size_t, size_t > > get_quotation_position_s;
		for( size_t index = 0; index < buff_size; ++index ) // 存在引号的情况
			if( HtmlStringTools::isQuotation( buff[ index ] ) ) {
				if( HtmlStringTools::jumpQuotation( buff, buff_size,
						index,
						get_quoation_position_end, get_quotation_position_s )
				) {
					size_t htmlStrBuffMaxnLen = get_quoation_position_end - index; // 不保存第一个引号
					HtmlChar *htmlStrBuff = new HtmlChar[ htmlStrBuffMaxnLen ];
					size_t htmlStrBuffIndex = 0;
					for( ++index; index < get_quoation_position_end & index < buff_size; ++index ) {
						if( HtmlStringTools::isSpace( buff[ index ] ) && htmlStrBuffIndex > 0 ) {
							auto htmlStringPtr = std::make_shared< HtmlString >( htmlStrBuff, htmlStrBuffIndex );
							auto iterator = resultVectorSPtr->begin( );
							auto end = resultVectorSPtr->end( );
							for( ; iterator != end; ++iterator )
								if( *iterator->get( ) == *htmlStringPtr )
									break;
							if( iterator == end )
								resultVectorSPtr->emplace_back( htmlStringPtr );
							htmlStrBuffIndex = 0;
							continue;
						}
						htmlStrBuff[ htmlStrBuffIndex ] = buff[ index ];
						++htmlStrBuffIndex;
					}
					index = get_quoation_position_end + 1; // 跳过引号
					if( htmlStrBuffIndex > 0 ) {
						auto htmlStringPtr = std::make_shared< HtmlString >( htmlStrBuff, htmlStrBuffIndex );
						auto iterator = resultVectorSPtr->begin( );
						auto end = resultVectorSPtr->end( );
						for( ; iterator != end; ++iterator )
							if( *iterator->get( ) == *htmlStringPtr )
								break;
						if( iterator == end )
							resultVectorSPtr->emplace_back( htmlStringPtr );
						continue;
					}
					delete[] htmlStrBuff;
					continue;
				}
				return nullptr; // 没有匹配的引号
			}
		if( resultVectorSPtr->size( ) == 0 ) {// 不存在引号的情况
			auto htmlStringPtr = std::make_shared< HtmlString >( buff, buff_size );
			resultVectorSPtr->emplace_back( htmlStringPtr );
		}
	} else {
		size_t start_index = 0, get_quoation_position_end;
		std::vector< std::pair< size_t, size_t > > get_quotation_position_s;
		if( HtmlStringTools::jumpQuotation( buff, buff_size, start_index, get_quoation_position_end, get_quotation_position_s ) ) {
			++start_index;
			HtmlString value = HtmlString( buff + start_index, get_quoation_position_end - start_index );
			cylHtmlTools::HtmlStringTools::removeBothSpace( value );
			auto strings = cylHtmlTools::HtmlStringTools::split( value, ' ' );
			for( auto &str : strings )
				resultVectorSPtr->emplace_back( std::make_shared< HtmlString >( str ) );
		} else {
			auto htmlStringPtr = std::make_shared< HtmlString >( buff, buff_size );
			resultVectorSPtr->emplace_back( htmlStringPtr );
		}
	}
	return resultVectorSPtr;
}
