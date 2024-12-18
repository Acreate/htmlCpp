#include "HtmlStringTools.h"

#include <cwctype>
#include <string>
#include <codecvt>
#include <locale>

using namespace cylHtmlTools;
bool HtmlStringTools::isSpace( const HtmlChar currentChar ) {
	return iswspace( currentChar ) || iswcntrl( currentChar );
}
bool HtmlStringTools::isSpace( const HtmlString &currentChar ) {
	for( auto &htmlChar : currentChar )
		if( !isSpace( htmlChar ) )
			return false;
	return true;
}
bool HtmlStringTools::isSpace( const std::string::value_type &currentChar ) {
	return isspace( currentChar ) || iscntrl( currentChar );
}

bool HtmlStringTools::jumpSingleQuotation( const HtmlChar *buff, const size_t buff_size, size_t start_index, size_t &get_quoation_position_end, std::vector< std::pair< size_t, size_t > > &get_quotation_position_s ) {
	if( buff[ start_index ] != charValue::singleQuotation )
		return false;
	size_t quotationFirstIndex = start_index;
	for( ++start_index; start_index < buff_size; ++start_index )
		if( buff[ start_index ] == charValue::singleQuotation )
			break;
		else if( buff[ start_index ] == charValue::doubleQuotation ) {
			if( !jumpDoubleQuotation( buff, buff_size, start_index, get_quoation_position_end, get_quotation_position_s ) )
				break;
			start_index = get_quoation_position_end;
		} else if( buff[ start_index ] == charValue::backSlash ) { // 是否出现转义字符
			auto newIndex = start_index + 1;
			if( buff[ newIndex ] == charValue::doubleQuotation
				|| buff[ newIndex ] == charValue::singleQuotation ) //转义字符则跳过一次
				start_index += 2;
		}
	if( start_index == buff_size ) // 超出下标
		return false;
	get_quoation_position_end = start_index;
	get_quotation_position_s.emplace_back( std::make_pair( quotationFirstIndex, get_quoation_position_end ) );
	return true;
}

bool HtmlStringTools::jumpDoubleQuotation( const HtmlChar *buff, const size_t buff_size, size_t start_index, size_t &get_quoation_position_end, std::vector< std::pair< size_t, size_t > > &get_quotation_position_s ) {
	if( buff[ start_index ] != charValue::doubleQuotation )
		return false;
	size_t quotationFirstIndex = start_index;
	for( ++start_index; start_index < buff_size; ++start_index )
		if( buff[ start_index ] == charValue::doubleQuotation )
			break;
		else if( buff[ start_index ] == charValue::singleQuotation ) {
			if( !jumpSingleQuotation( buff, buff_size, start_index, get_quoation_position_end, get_quotation_position_s ) )
				break;
			start_index = get_quoation_position_end;
		} else if( buff[ start_index ] == charValue::backSlash ) { // 是否出现转义字符
			auto newIndex = start_index + 1;
			if( buff[ newIndex ] == charValue::doubleQuotation
				|| buff[ newIndex ] == charValue::singleQuotation ) //转义字符则跳过一次
				start_index += 2;
		}
	if( start_index == buff_size )// 超出下标
		return false;
	get_quoation_position_end = start_index;
	get_quotation_position_s.emplace_back( std::make_pair( quotationFirstIndex, get_quoation_position_end ) );
	return true;
}
bool HtmlStringTools::isQuotation( const HtmlChar &char_value ) {
	if( char_value == charValue::doubleQuotation || char_value == charValue::singleQuotation )
		return true;
	return false;
}

bool HtmlStringTools::jumpQuotation( const HtmlChar *buff, const size_t buff_size, size_t start_index, size_t &get_quoation_position_end, std::vector< std::pair< size_t, size_t > > &get_quotation_position_s ) {
	if( buff[ start_index ] == charValue::doubleQuotation )
		return jumpDoubleQuotation( buff, buff_size, start_index, get_quoation_position_end, get_quotation_position_s );
	else if( buff[ start_index ] == charValue::singleQuotation )
		return jumpSingleQuotation( buff, buff_size, start_index, get_quoation_position_end, get_quotation_position_s );
	else
		return false;
}
bool HtmlStringTools::jumpSingleQuotation( const HtmlChar *buff, size_t buff_size, size_t start_index, size_t &get_quoation_position_end ) {
	if( buff[ start_index ] != charValue::singleQuotation )
		return false;
	for( ++start_index; start_index < buff_size; ++start_index )
		if( buff[ start_index ] == charValue::singleQuotation )
			break;
		else if( buff[ start_index ] == charValue::doubleQuotation ) {
			if( !jumpDoubleQuotation( buff, buff_size, start_index, get_quoation_position_end ) )
				break;
			start_index = get_quoation_position_end;
		} else if( buff[ start_index ] == charValue::backSlash ) { // 是否出现转义字符
			auto newIndex = start_index + 1;
			if( buff[ newIndex ] == charValue::doubleQuotation
				|| buff[ newIndex ] == charValue::singleQuotation ) //转义字符则跳过一次
				start_index += 2;
		}
	if( start_index == buff_size ) // 超出下标
		return false;
	get_quoation_position_end = start_index;
	return true;
}
bool HtmlStringTools::jumpDoubleQuotation( const HtmlChar *buff, size_t buff_size, size_t start_index, size_t &get_quoation_position_end ) {
	if( buff[ start_index ] != charValue::doubleQuotation )
		return false;
	for( ++start_index; start_index < buff_size; ++start_index )
		if( buff[ start_index ] == charValue::doubleQuotation )
			break;
		else if( buff[ start_index ] == charValue::singleQuotation ) {
			if( !jumpSingleQuotation( buff, buff_size, start_index, get_quoation_position_end ) )
				break;
			start_index = get_quoation_position_end;
		} else if( buff[ start_index ] == charValue::backSlash ) { // 是否出现转义字符
			auto newIndex = start_index + 1;
			if( buff[ newIndex ] == charValue::doubleQuotation
				|| buff[ newIndex ] == charValue::singleQuotation ) //转义字符则跳过一次
				start_index += 2;
		}
	if( start_index == buff_size )// 超出下标
		return false;
	get_quoation_position_end = start_index;
	return true;
}
bool HtmlStringTools::jumpQuotation( const HtmlChar *buff, const size_t buff_size, size_t start_index, size_t &get_quoation_position_end ) {
	if( buff[ start_index ] == charValue::doubleQuotation )
		return jumpDoubleQuotation( buff, buff_size, start_index, get_quoation_position_end );
	else if( buff[ start_index ] == charValue::singleQuotation )
		return jumpSingleQuotation( buff, buff_size, start_index, get_quoation_position_end );
	else
		return false;
}
bool HtmlStringTools::isRouteChar( HtmlChar currentChar ) {
	return currentChar == charValue::forwardSlash || currentChar == charValue::backSlash;
}
bool HtmlStringTools::jumpSace( const HtmlChar *foreachWCStr, size_t foreachMaxIndex, size_t *startIndex ) {
	for( ; *startIndex < foreachMaxIndex; ++( *startIndex ) )
		if( !isSpace( foreachWCStr[ *startIndex ] ) )
			return true;
	return false;
}
bool HtmlStringTools::findNextHtmlCharPotion( const HtmlChar *w_c_ptr, size_t maxIndex, const HtmlChar find_w_c_char, size_t &startIndex ) {
	for( ; startIndex < maxIndex; ++startIndex )
		if( w_c_ptr[ startIndex ] == find_w_c_char )
			return true;
	return false;
}


bool HtmlStringTools::findNextHtmlStringPotion( const HtmlChar *w_c_ptr, size_t src_w_c_str_len, size_t startIndex, const HtmlChar *find_w_c_string, size_t find_w_c_str_len, size_t maxIndex, size_t *result ) {

	if( src_w_c_str_len == 0 )
		maxIndex = HtmlStringLen( w_c_ptr );
	else if( maxIndex == 0 )
		maxIndex = src_w_c_str_len;
	if( find_w_c_str_len == 0 )
		find_w_c_str_len = HtmlStringLen( find_w_c_string );

	if( src_w_c_str_len < find_w_c_str_len )
		return false;
	size_t leftIndex = startIndex;
	size_t rightIndex = 0;
	wchar_t leftChar, rightChar;
	for( ; leftIndex < maxIndex; ++leftIndex ) {
		leftChar = w_c_ptr[ leftIndex ];
		rightChar = find_w_c_string[ rightIndex ];
		if( leftChar == rightChar )
			do {
				rightIndex++;
				leftChar = w_c_ptr[ leftIndex + rightIndex ];
				rightChar = find_w_c_string[ rightIndex ];
				if( rightChar == '\0' || rightIndex == find_w_c_str_len )
					break;
				else if( leftChar != rightChar ) {
					rightIndex = 0;
					break;
				}
			} while( true );
		if( rightIndex ) {
			if( result )
				*result = leftIndex - startIndex;
			return true;
		}
	}
	return false;
}

bool HtmlStringTools::equHtmlString( const HtmlString &left, const HtmlString &right ) {
	if( &left == &right )
		return true;
	size_t leftLen = left.length( );
	size_t rightLen = right.length( );
	if( leftLen != rightLen )
		return false;
	for( size_t index = 0; index < leftLen; ++index )
		if( left[ index ] != right[ index ] )
			return false;
	return true;
}

HtmlString & HtmlStringTools::removeLeftSpace( HtmlString &str ) {
	size_t index = 0, leftLen = str.length( );
	for( ; index < leftLen; ++index )
		if( !isSpace( str[ index ] ) )
			break;
	if( index == leftLen )
		str = HtmlString( );
	else
		str = str.substr( index );
	return str;
}
HtmlString & HtmlStringTools::removeAllSpace( HtmlString &str ) {

	// 删除 left 字符串
	size_t leftLen = str.length( );
	HtmlChar *buff = new HtmlChar[ leftLen ];
	size_t index = 0, buffIndex = 0;
	for( ; index < leftLen; ++index ) {
		auto value = str.at( index );
		if( HtmlStringTools::isSpace( value ) )
			continue;
		buff[ buffIndex ] = value;
		++buffIndex;
	}
	str = HtmlString( buff, buffIndex );
	delete[] buff;
	return str;
}
HtmlString & HtmlStringTools::removeRightSpace( HtmlString &str ) {
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
	return str;
}

HtmlString & HtmlStringTools::removeBothSpace( HtmlString &str ) {
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
	return str;
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
std::vector< HtmlString > HtmlStringTools::split( const HtmlString &src_html_string, const HtmlChar split_char ) {

	size_t strLen = src_html_string.size( );
	if( strLen == 0 )
		return { src_html_string };
	std::vector< HtmlString > result;

	HtmlChar *buff = new HtmlChar[ strLen ];
	size_t buffIndex = 0, spliteIndex = 0;
	for( ; spliteIndex < strLen; ++spliteIndex ) {
		HtmlChar value = src_html_string[ spliteIndex ];
		if( value == split_char ) {
			if( buffIndex == 0 )
				continue;
			HtmlString subStr( buff, buffIndex );
			result.emplace_back( subStr );
			buffIndex = 0;
		} else {
			buff[ buffIndex ] = value;
			++buffIndex;
		}
	}
	if( buffIndex != 0 ) {
		HtmlString subStr( buff, buffIndex );
		result.emplace_back( subStr );
	}

	return result;
}
std::vector< HtmlString > HtmlStringTools::vectorStrAdjustSubStr( std::vector< HtmlString > &str_vector ) {
	if( str_vector.size( ) < 2 )
		return str_vector;
	// 长度排序
	std::list< HtmlString > lenSort;
	for( auto &str : str_vector ) {
		auto iterator = lenSort.begin( ), end = lenSort.end( );
		for( ; iterator != end; ++iterator )
			if( iterator->length( ) < str.length( ) )
				break;
		lenSort.insert( iterator, str );
	}
	std::vector< HtmlString > clone( lenSort.begin( ), lenSort.end( ) );
	lenSort.clear( );
	size_t listCount = clone.size( );
	size_t index = 0;
	size_t subIndex = 0;
	for( ; index < listCount; ++index ) {
		HtmlString &currentStr = clone[ index ];
		subIndex = index + 1;
		for( ; subIndex < listCount; ++subIndex )
			if( findNextHtmlStringPotion( &currentStr, &clone[ subIndex ] ) )
				break;
		if( subIndex == listCount )
			lenSort.insert( lenSort.begin( ), currentStr );
	}
	return std::vector< HtmlString >( lenSort.begin( ), lenSort.end( ) );
}
bool HtmlStringTools::lastFindChar( const HtmlString &find_string, const HtmlString::value_type &find_target_char, size_t *index ) {
	size_t length = find_string.length( );
	while( true ) {
		wchar_t value = find_string[ length ];
		if( value == find_target_char ) {
			*index = length;
			return true;
		}
		if( length == 0 )
			return false;
		--length;
	}
	return false;
}
HtmlString HtmlStringTools::mid( const HtmlString &mid_string, const size_t start_index, const size_t n_len ) {
	size_t length = mid_string.length( );
	if( length < start_index )
		return mid_string;
	auto iterator = mid_string.begin( ) + start_index;
	auto end = mid_string.end( );
	HtmlString::value_type *buff = new HtmlString::value_type[ n_len + 1 ];
	length = 0;
	do {
		auto value = *iterator;
		buff[ length ] = value;
		if( value == 0 || length == n_len )
			break;

		++length;
		++iterator;
		if( iterator == end ) {
			buff[ length ] = 0;
			break;
		}
	} while( true );
	HtmlString result( buff, length );
	delete buff;
	return result;
}
HtmlString HtmlStringTools::mid( const HtmlString &mid_string, const size_t start_index ) {
	return HtmlStringTools::mid( mid_string, start_index, mid_string.length( ) );
}
std::string & HtmlStringTools::removeAllSpace( std::string &str ) {
	size_t leftLen = str.length( );
	std::string::value_type *buff = new std::string::value_type [ leftLen ];
	size_t index = 0, buffIndex = 0;
	for( ; index < leftLen; ++index ) {
		auto value = str.at( index );
		if( HtmlStringTools::isSpace( value ) )
			continue;
		buff[ buffIndex ] = value;
		++buffIndex;
	}
	str = std::string( buff, buffIndex );
	delete[] buff;
	return str;
}
