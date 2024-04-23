﻿#include "WStrTools.h"

#include <cwctype>
#include <string>
#include <codecvt>
#include <locale>
using namespace HtmlTools;
bool WStrTools::isJumpSpace( wchar_t currentChar ) {
	return iswspace( currentChar ) || iswcntrl( currentChar ) || iswcntrl( currentChar );
}
bool WStrTools::jimpSace( const wchar_t *foreachWCStr, size_t foreachMaxIndex, size_t *startIndex ) {
	for( ; *startIndex < foreachMaxIndex; ++( *startIndex ) )
		if( !isJumpSpace( foreachWCStr[ *startIndex ] ) )
			return true;
	return false;
}
bool WStrTools::findNextWCharPotion( const wchar_t *w_c_ptr, size_t maxIndex, const wchar_t find_w_c_char, size_t *startIndex ) {
	for( ; *startIndex < maxIndex; ++( *startIndex ) )
		if( w_c_ptr[ *startIndex ] == find_w_c_char )
			return true;
	return false;
}

bool WStrTools::findNextWStringPotion( const wchar_t *w_c_ptr, size_t src_w_c_str_len, size_t startIndex, const wchar_t *find_w_c_string, size_t find_w_c_str_len, size_t maxIndex, size_t *result ) {

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
std::wstring WStrTools::stringConverWString( const std::string &str ) {
	std::wstring_convert< std::codecvt_utf8< wchar_t > > converter;
	return converter.from_bytes( str );
}
std::string WStrTools::wstringConverString( const std::wstring &wstr ) {

	std::wstring_convert< std::codecvt_utf8< wchar_t > > converter;
	return converter.to_bytes( wstr );
}