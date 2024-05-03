#include "XDir.h"

#include <iostream>

#include "../../htmlString/HtmlStringTools.h"
#include "XDirAttribute.h"
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
	const HtmlChar *buff, const size_t buff_size,
	size_t current_index,
	size_t &get_str_start, size_t &get_str_end
) {
	for( ; current_index < buff_size; ++current_index )  // 获取第一个非空
		if( !HtmlStringTools::isSpace( buff[ current_index ] ) )
			break;
	get_str_start = current_index;
	for( ; current_index < buff_size; ++current_index )  // 查找等号
		if( buff[ current_index ] == charValue::equ ) {
			get_str_end = current_index;
			break;
		} else if( buff[ current_index ] == charValue::singleQuotation
			|| buff[ current_index ] == charValue::doubleQuotation ) { // 跳过匹配的双引号
			get_str_start = current_index;
			std::vector< std::pair< size_t, size_t > > getQuoattionPostions;
			if( !HtmlStringTools::jumpQuotation( buff, buff_size, current_index, get_str_end, getQuoattionPostions ) )
				return false;
			current_index = get_str_end + 1;
			while( current_index < buff_size && HtmlStringTools::isSpace( buff[ current_index++ ] ) )
				continue;
			get_str_end = current_index;
			if( current_index == buff_size )
				return true; // 超出遍历界限
		}
	for( ; get_str_end > get_str_start; --get_str_end )
		if( buff[ get_str_end ] != charValue::equ && !HtmlStringTools::isSpace( buff[ get_str_end ] ) )
			break;
	if( get_str_end == get_str_start )
		return false;
	return true;
}

/// <summary>
/// 解析字符串当中的属性
/// </summary>
/// <param name="buff">缓冲</param>
/// <param name="buff_size">缓冲长度</param>
/// <param name="save_vector_xdirattribute_s">解析对象存储位置</param>
/// <returns>解析个数</returns>
inline size_t parseXDirAttributes( const HtmlChar *buff, const size_t buff_size,
	Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s ) {
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
			nameRight += 1;
			auto findStart = nameRight;
			for( ; findStart < buff_size; ++findStart ) // 找到等号
				if( buff[ findStart ] == charValue::equ )
					break;
			++findStart;
			//// 找到属性值 (匹配的 valueLeft, valueRight)
			if( !findStr( buff, buff_size, findStart, valueLeft, valueRight ) )
				break;
			index = valueRight;
			++valueRight;
			auto xDirAttributeShared( std::make_shared< XDirAttribute >( buff + nameLeft, nameRight - nameLeft,
				buff + valueLeft, valueRight - valueLeft
			) );
			auto iterator = save_vector_xdirattribute_s->begin( );
			auto end = save_vector_xdirattribute_s->end( );
			for( ; iterator != end; ++iterator )
				if( iterator->get( )->getName( ) == xDirAttributeShared->getName( ) ) { // 是否存在相同而的属性
					auto values = iterator->get( )->getValues( );
					auto srcValues = xDirAttributeShared->getValues( );
					for( auto &srcValue : *srcValues ) {
						auto desIterator = values->begin( );
						auto desEnd = values->end( );
						for( ; desIterator != desEnd; ++desIterator )
							if( *desIterator->get( ) == *srcValue.get( ) )
								break;
						if( desIterator == desEnd )
							values->emplace_back( srcValue );
					}
					break;
				}
			if( iterator == end ) // 不存在，则加入
				save_vector_xdirattribute_s->emplace_back( xDirAttributeShared );
		}
	}

	return parseCount;
}
XDir::XDir( const HtmlString &param ) {
	size_t length = param.length( );
	if( length == 0 )
		return;
	auto data = param.data( );
	HtmlChar value; // 当前校验值
	HtmlChar *buff = new HtmlChar[ length + 1 ]; // 缓存指针
	size_t buffIndex = 0; // 缓存下标
	Vector_XDirAttributeSPtr_Shared attributesResult; // 存储解析后的属性
	for( size_t index = 0; index < length; ++index ) {
		value = data[ index ];
		if( value == L'[' ) { // 找到 ]
			HtmlString xdirName( buff, buffIndex );
			namesList.emplace_back( xdirName );
			buffIndex = 0;
			++index;
			for( ; index < length; ++index ) {
				value = data[ index ];
				if( value == L']' ) { // 找到 ]
					if( buffIndex > 0 ) {
						// 分解属性
						attributesResult->clear( );
						auto projectCount = parseXDirAttributes( buff, buffIndex, attributesResult );
						if( projectCount ) {
							HtmlString attributeTypeName;
							HtmlString attributeTypeValue;
							insertXDirAttributeInfo( xdirName, attributeTypeName, attributeTypeValue );
						}
						buffIndex = 0;
					}
					break;
				} else if( value == charValue::singleQuotation || value == charValue::doubleQuotation ) {
					size_t getQuotationEnd;
					std::vector< std::pair< size_t, size_t > > getQuoattionPostions;
					if( HtmlStringTools::jumpQuotation( data, length, index, getQuotationEnd, getQuoattionPostions ) ) {
						buff[ buffIndex ] = value;
						++buffIndex;
						for( ++index; index < getQuotationEnd; ++index, ++buffIndex )
							buff[ buffIndex ] = data[ index ];
						buff[ buffIndex ] = data[ index ];
						++buffIndex;
					}
					continue;
				}
				buff[ buffIndex ] = value;
				++buffIndex;
			}
			continue;
		}
		buff[ buffIndex ] = value;
		++buffIndex;
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
		//Vector_HtmlStringSPtr outHtmlStringShared;
		//if( tryGet( attributesList, name, outHtmlStringShared ) ) { // 存在
		//	result.append( L"[" );
		//	for( auto &attribute : outHtmlStringShared )
		//		result.append( *attribute );
		//	result.append( L"]" );
		//}
	}
	return result;
}

HtmlString XDir::getDirName( ) const {
	HtmlString result;
	for( auto &name : namesList )
		result.append( name );
	return result;
}
