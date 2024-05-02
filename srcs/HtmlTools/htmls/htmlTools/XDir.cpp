#include "XDir.h"

#include <iostream>

#include "../../htmlString/HtmlStringTools.h"
using namespace cylHtmlTools;
XDir::XDir_Attribute_Status XDir::insertXDirAttributeInfo( const HtmlString &dir_set_name, const HtmlString &xdir_set_type_name, const HtmlString &xDir_set_attribute_value ) {
	return { };
}
XDir::XDir_Attribute_Status XDir::tryAttributeGet( const HtmlString &dir_set_name, const HtmlString &xdir_set_type_name, Vector_HtmlStringSPtr &xDir_result_attribute_value ) {
	return { };
}
/// <summary>
/// 找到下一个字符串<br/>
/// 字符串两侧不存在空格（如 R(" abd es")，则返回 abd，如果需要空格，则需要使用单引号或者双引号）
/// </summary>
/// <param name="buff">检查缓冲</param>
/// <param name="buff_size">缓冲大小</param>
/// <param name="current_index">缓冲开始位置</param>
/// <param name="get_str_start">返回的字符串开始位置</param>
/// <param name="get_str_end">返回的字符串结束位置</param>
/// <returns></returns>
inline bool findStr(
	HtmlChar *buff, const size_t buff_size, size_t current_index, size_t &get_str_start, size_t &get_str_end
) {

	for( ; current_index < buff_size; ++current_index )  // 获取第一个非空
		if( !HtmlStringTools::isSpace( buff[ current_index ] ) )
			break;
	for( ; current_index < buff_size; ++current_index )  // 查找等号
		if( buff[ current_index ] == charValue::equ ) {
			break;
		} else if( buff[ current_index ] == charValue::singleQuotation
			|| buff[ current_index ] == charValue::doubleQuotation ) { // 跳过匹配的双引号
			size_t getQuotation;
			std::vector< std::pair< size_t, size_t > > getQuoattionPostions;
			if( !HtmlStringTools::jumpQuotation( buff, buff_size, current_index, getQuotation, getQuoattionPostions ) )
				return false;
		}

	return true;
}

/// <summary>
/// 解析字符串当中的属性
/// </summary>
/// <param name="buff">缓冲</param>
/// <param name="buff_size">缓冲长度</param>
/// <param name="save_vector_html_str_s">解析对象存储位置</param>
/// <returns>解析个数</returns>
inline size_t parse( HtmlChar *buff, size_t buff_size, HtmlStringMapToVectorHtmlStrSPtr &save_vector_html_str_s ) {
	size_t parseCount = 0;
	HtmlChar charValue;
	size_t index = 0;
	size_t nameLeft, nameRight;
	size_t valueLeft, valueRight;
	HtmlString designatedAttributeAtClass( L"class" );
	for( ; index < buff_size; ++index ) {
		charValue = buff[ index ];
		if( charValue == charValue::at ) { // 找到了 @ 符号
			//// 找到属性名称 (匹配的 nameLeft, nameRight)
			if( !findStr( buff, buff_size, index + 1, nameLeft, nameRight ) )
				break;
			//// 找到属性值 (匹配的 valueLeft, valueRight)
			if( !findStr( buff, buff_size, nameRight + 1, valueLeft, valueRight ) )
				break;
			index = valueRight;

			HtmlString attributeName( buff, nameLeft, nameRight - nameLeft );
			if( HtmlStringTools::equHtmlString( attributeName, designatedAttributeAtClass ) ) { // 如果是属性的话，需要二次分解

				continue;
			}

			auto iterator = save_vector_html_str_s.begin( );
			auto end = save_vector_html_str_s.end( );
			for( ; iterator != end; ++iterator )
				if( iterator->first == attributeName ) {// 存在属性名称
					iterator->second->emplace_back( std::make_shared< HtmlString >( buff, valueLeft, valueRight - valueLeft ) );
					break;
				}
			if( iterator == end ) { // 不存在睡醒
				Vector_HtmlStringSPtr_Shared vectorHtmlStringSPtr( std::make_shared< Vector_HtmlStringSPtr >( ) );
				vectorHtmlStringSPtr->emplace_back( std::make_shared< HtmlString >( buff, valueLeft, valueRight - valueLeft ) );
				save_vector_html_str_s.insert( std::make_pair( attributeName, vectorHtmlStringSPtr ) );
			}
		}
	}

	return parseCount;
}
XDir::XDir( const HtmlString &param ) {
	size_t length = param.length( );
	if( length == 0 )
		return;
	HtmlChar value;
	HtmlChar *buff = new HtmlChar[ length + 1 ];
	size_t buffIndex = 0;
	HtmlStringMapToVectorHtmlStrSPtr attributesResult;
	for( size_t index = 0; index < length; ++index ) {
		value = param[ index ];
		if( value == L'[' ) { // 找到 ]
			HtmlString name( buff, buffIndex );
			namesList.emplace_back( name );
			buffIndex = 0;
			++index;
			for( ; index < length; ++index ) {
				value = param[ index ];
				if( value == L']' ) { // 找到 ]
					if( buffIndex > 0 ) {
						// 分解属性
						attributesResult.clear( );
						auto projectCount = parse( buff, buffIndex, attributesResult );
						if( projectCount ) {
							HtmlString attributeTypeName;
							HtmlString attributeTypeValue;
							insertXDirAttributeInfo( name, attributeTypeName, attributeTypeValue );
						}
						buffIndex = 0;
					}
					break;
				}
				if( value == charValue::doubleQuotation ) {
					buff[ buffIndex ] = value;
					++index;
					++buffIndex;
					for( ; index < length; ++index ) {
						buff[ buffIndex ] = value = param[ index ];
						++buffIndex;
						if( value == charValue::doubleQuotation )
							break;
					}
					continue;
				}
				if( value == charValue::singleQuotation ) {
					buff[ buffIndex ] = value = param[ index ];
					++buffIndex;
					for( ; index < length; ++index ) {
						buff[ buffIndex ] = value = param[ index ];
						++buffIndex;
						if( param[ index ] == charValue::singleQuotation )
							break;
					}
					continue;
				}
				buff[ buffIndex ] = value;
				++buffIndex;
			}
			continue;
		}
		buff[ buffIndex ] = value;
		++buff;
	}
	if( buffIndex > 0 ) {
		HtmlString name( buff, buffIndex );
		namesList.emplace_back( name );
	}
	delete buff;
}
XDir::~XDir( ) {
}
bool XDir::hasName( const HtmlString &dir_name ) const {
	for( auto &name : namesList )
		if( HtmlStringTools::equRemoveSpaceOverHtmlString( dir_name, name ) )
			return true;
	return false;
}


bool XDir::hasAttribute( const HtmlStringPairUnorderMap_Shared &attribute, const HtmlString &nodeName ) {

	return true;
}
HtmlString XDir::getXDirName( ) const {
	HtmlString result;
	for( auto &name : namesList ) {
		result.append( name );
		Vector_HtmlStringSPtr outHtmlStringShared;
		if( tryGet( attributesList, name, outHtmlStringShared ) ) { // 存在
			result.append( L"[" );
			for( auto &attribute : outHtmlStringShared )
				result.append( *attribute );
			result.append( L"]" );
		}
	}
	return result;
}

HtmlString XDir::getDirName( ) const {
	HtmlString result;
	for( auto &name : namesList )
		result.append( name );
	return result;
}
