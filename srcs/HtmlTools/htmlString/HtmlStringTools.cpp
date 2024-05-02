#include "HtmlStringTools.h"

#include <cwctype>
#include <string>
#include <codecvt>
#include <locale>
using namespace cylHtmlTools;
bool HtmlStringTools::isSpace( HtmlChar currentChar ) {
	return iswspace( currentChar ) || iswcntrl( currentChar ) || iswcntrl( currentChar );
}
bool HtmlStringTools::jumpQuotation( HtmlChar *buff, const size_t buff_size, size_t start_index, size_t &get_quoation_position_end, std::vector< std::pair< size_t, size_t > > &get_quotation_position_s ) {
	size_t doubleQuotationFirstIndex = 0, doubleQuotationEndIndex = 0;
	do {
		if( buff[ start_index ] == charValue::doubleQuotation ) {
			for( ++start_index; start_index < buff_size; ++start_index )
				if( buff[ start_index ] == charValue::doubleQuotation )
					break;
				else if( buff[ start_index ] == charValue::singleQuotation ) {
					if( !jumpQuotation( buff, buff_size, start_index, get_quoation_position_end, get_quotation_position_s ) )
						break;
					start_index = get_quoation_position_end;
				}
			if( start_index == buff_size )
				break;
			doubleQuotationEndIndex = start_index;
			get_quotation_position_s.emplace_back( std::make_pair( doubleQuotationFirstIndex, doubleQuotationEndIndex ) );
		} else if( buff[ start_index ] == charValue::singleQuotation ) {
			for( ++start_index; start_index < buff_size; ++start_index )
				if( buff[ start_index ] == charValue::singleQuotation )
					break;
				else if( buff[ start_index ] == charValue::doubleQuotation ) {
					if( !jumpQuotation( buff, buff_size, start_index, get_quoation_position_end, get_quotation_position_s ) )
						break;
					start_index = get_quoation_position_end;
				}
			if( start_index == buff_size )
				break;
			doubleQuotationEndIndex = start_index;
			get_quotation_position_s.emplace_back( std::make_pair( doubleQuotationFirstIndex, doubleQuotationEndIndex ) );
		} else
			return false;
	} while( false );

	if( doubleQuotationFirstIndex > doubleQuotationEndIndex )
		return false;
	return true;
}
bool HtmlStringTools::isRouteChar( HtmlChar currentChar ) {
	return currentChar == charValue::forwardSlash || currentChar == charValue::backSlash;
}
bool HtmlStringTools::jimpSace( const HtmlChar *foreachWCStr, size_t foreachMaxIndex, size_t *startIndex ) {
	for( ; *startIndex < foreachMaxIndex; ++( *startIndex ) )
		if( !isSpace( foreachWCStr[ *startIndex ] ) )
			return true;
	return false;
}
bool HtmlStringTools::findNextWCharPotion( const HtmlChar *w_c_ptr, size_t maxIndex, const HtmlChar find_w_c_char, size_t *startIndex ) {
	for( ; *startIndex < maxIndex; ++( *startIndex ) )
		if( w_c_ptr[ *startIndex ] == find_w_c_char )
			return true;
	return false;
}

bool HtmlStringTools::findNextWStringPotion( const HtmlChar *w_c_ptr, size_t src_w_c_str_len, size_t startIndex, const HtmlChar *find_w_c_string, size_t find_w_c_str_len, size_t maxIndex, size_t *result ) {

	if( src_w_c_str_len < find_w_c_str_len )
		return false;
	find_w_c_str_len -= 1;
	size_t leftIndex = startIndex;
	size_t rightIndex = 0;
	wchar_t leftChar, rightChar;
	for( ; leftIndex < maxIndex; ++leftIndex ) {
		leftChar = w_c_ptr[ leftIndex ];
		rightChar = find_w_c_string[ rightIndex ];
		do {
			if( leftChar != rightChar ) {
				rightIndex = 0;
				break;
			} else if( rightChar == '\0' || rightIndex == find_w_c_str_len )
				break;
			rightIndex++;
			leftChar = w_c_ptr[ leftIndex + rightIndex ];
			rightChar = find_w_c_string[ rightIndex ];
		} while( true );
		if( rightIndex ) {
			*result = leftIndex - startIndex;
			return true;
		}
	}
	return false;
}
bool HtmlStringTools::equHtmlString( const HtmlString &left, const HtmlString &right ) {
	size_t leftLen = left.length( );
	size_t rightLen = right.length( );
	if( leftLen != rightLen )
		return false;
	for( size_t index = 0; index < leftLen; ++index )
		if( left[ index ] != right[ index ] )
			return false;
	return true;
}

void HtmlStringTools::removeLeftSpace( HtmlString &str ) {
	size_t index = 0, leftLen = str.length( );
	for( ; index < leftLen; ++index )
		if( !isSpace( str[ index ] ) )
			break;
	if( index == leftLen )
		str = HtmlString( );
	else
		str = str.substr( index );
}
void HtmlStringTools::removeRightSpace( HtmlString &str ) {
	// 删除 left 字符串
	size_t leftLen = str.length( );
	while( 0 < leftLen ) {
		if( !HtmlStringTools::isSpace( str[ leftLen - 1 ] ) ) {
			str = str.substr( 0, leftLen );
			break;
		}
		if( leftLen == 0 ) {
			str = HtmlString( );
			break;
		}
		--leftLen;
	}
}

void HtmlStringTools::removeBothSpace( HtmlString &str ) {
	// 删除 str 字符串
	size_t index = 0, leftLen = str.length( );
	for( ; index < leftLen; ++index )
		if( !isSpace( str[ index ] ) ) {
			while( index < leftLen ) {
				if( !isSpace( str[ leftLen - 1 ] ) ) {
					str = str.substr( index, leftLen - index );
					break;
				}
				if( leftLen == index ) {
					str = HtmlString( );
					break;
				}
				--leftLen;
			}
			break;
		}
}
bool HtmlStringTools::equRemoveSpaceOverHtmlString( HtmlString leftStr, HtmlString rightStr, RemoveSpaceStatus removeSpaceStatus ) {
	switch( removeSpaceStatus ) {
	case left :
		removeLeftSpace( leftStr );
		removeLeftSpace( rightStr );
		break;
	case right :
		removeRightSpace( leftStr );
		removeRightSpace( rightStr );
		break;
	case both :
		removeBothSpace( leftStr );
		removeBothSpace( rightStr );
		break;
	}
	return equHtmlString( leftStr, rightStr );
}
