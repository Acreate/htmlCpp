#include "HtmlStringTools.h"

#include <cwctype>
#include <string>
#include <codecvt>
#include <locale>
using namespace htmlTools;
bool HtmlStringTools::isJumpSpace( HtmlChar currentChar ) {
	return iswspace( currentChar ) || iswcntrl( currentChar ) || iswcntrl( currentChar );
}
bool HtmlStringTools::isRouteChar( HtmlChar currentChar ) {
	return currentChar == charValue::forwardSlash || currentChar == charValue::backSlash;
}
bool HtmlStringTools::jimpSace( const HtmlChar *foreachWCStr, size_t foreachMaxIndex, size_t *startIndex ) {
	for( ; *startIndex < foreachMaxIndex; ++( *startIndex ) )
		if( !isJumpSpace( foreachWCStr[ *startIndex ] ) )
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
bool HtmlStringTools::equHtmlString( HtmlString &left, HtmlString &right ) {
	size_t leftLen = left.length( );
	size_t rightLen = right.length( );
	if( leftLen != rightLen )
		return false;
	for( size_t index = 0; index < leftLen; ++index )
		if( left[ index ] != right[ index ] )
			return false;
	return true;
}


bool HtmlStringTools::equRemoveSpaceOverHtmlString( HtmlString left, HtmlString right, RemoveSpaceStatus removeSpaceStatus ) {
	switch( removeSpaceStatus ) {
	case RemoveSpaceStatus::left : {
		// 删除 left 字符串
		size_t index = 0, leftLen = left.length( ), rightLen = right.length( );
		for( ; index < leftLen; ++index )
			if( !isJumpSpace( left[ index ] ) )
				break;
		if( index == leftLen )
			left = HtmlString( );
		else
			left = left.substr( index );

		// 删除 right 字符串
		for( index = 0; index < rightLen; ++index )
			if( !isJumpSpace( right[ index ] ) )
				break;
		if( index == rightLen )
			right = HtmlString( );
		else
			right = right.substr( index );
	}
	case RemoveSpaceStatus::right : {
		// 删除 left 字符串
		size_t leftLen = left.length( ), rightLen = right.length( );
		while( 0 < leftLen ) {
			if( !isJumpSpace( left[ leftLen ] ) )
				break;
			if( leftLen == 0 )
				break;
			--leftLen;
		}
		if( 0 == leftLen )
			left = HtmlString( );
		else
			left = left.substr( 0, leftLen );

		// 删除 right 字符串
		while( 0 < rightLen ) {
			if( !isJumpSpace( right[ rightLen ] ) )
				break;
			if( rightLen == 0 )
				break;
			--rightLen;
		}
		if( 0 == rightLen )
			right = HtmlString( );
		else
			right = right.substr( 0, rightLen );
	}
	case RemoveSpaceStatus::both : {
		// 删除 left 字符串
		size_t index = 0, leftLen = left.length( ), rightLen = right.length( );
		for( ; index < leftLen; ++index )
			if( !isJumpSpace( left[ index ] ) ) {
				while( index < leftLen ) {
					if( !isJumpSpace( left[ leftLen ] ) )
						break;
					if( leftLen == index )
						break;
					--leftLen;
				}
				break;
			}
		if( index == leftLen )
			left = HtmlString( );
		else
			left = left.substr( index, leftLen - index );

		// 删除 right 字符串
		for( index = 0; index < rightLen; ++index )
			if( !isJumpSpace( right[ index ] ) ) {
				while( index < rightLen ) {
					if( !isJumpSpace( right[ rightLen ] ) )
						break;
					if( rightLen == index )
						break;
					--rightLen;
				}
				break;
			}
		if( index == rightLen )
			right = HtmlString( );
		else
			right = right.substr( index, rightLen - index );
	}
	}
	return equHtmlString( left, right );
}
