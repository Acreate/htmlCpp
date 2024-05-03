#include "XDir.h"

#include <iostream>

#include "../../htmlString/HtmlStringTools.h"
#include "XDirAttribute.h"
using namespace cylHtmlTools;
inline void appendVector( Vector_HtmlStringSPtr_Shared &src, Vector_HtmlStringSPtr_Shared &des ) {
	for( auto &value : *des ) {
		auto iterator = src->begin( );
		auto end = src->end( );
		for( ; iterator != end; ++iterator )
			if( HtmlStringTools::equRemoveSpaceOverHtmlString( *iterator->get( ), *value.get( ) ) )
				break;
		if( iterator == end )
			src->emplace_back( value );
	}
}
XDir::XDir_Attribute_Status XDir::insertXDirAttributeInfo( const HtmlString &dir_set_name, XDirAttribute_Shared &xdir_info ) {
	auto mapIterator = attributesMap->begin( );
	auto mapEnd = attributesMap->end( );
	for( ; mapIterator != mapEnd; ++mapEnd )
		if( HtmlStringTools::equRemoveSpaceOverHtmlString( mapIterator->first, dir_set_name ) ) { // 存在
			auto &attributes = mapIterator->second;
			auto iterator = attributes->begin( );
			auto end = attributes->end( );
			auto name = *xdir_info->name;
			for( ; iterator != end; ++iterator )
				if( HtmlStringTools::equRemoveSpaceOverHtmlString( *iterator->get( )->name, name ) ) {
					appendVector( iterator->get( )->values, xdir_info->values );
					return Append_Element;
				}
			if( iterator == end )
				attributes->emplace_back( xdir_info );
			return Add_Element;
		}
	if( mapIterator == mapEnd ) {
		Vector_XDirAttributeSPtr_Shared vectorXDirAttributeSPtrShared( std::make_shared< Vector_XDirAttributeSPtr >( ) );
		vectorXDirAttributeSPtrShared->emplace_back( xdir_info );
		attributesMap->emplace( dir_set_name, vectorXDirAttributeSPtrShared );
		return Add_Array;
	}
	return None;
}
XDir::XDir_Attribute_Status XDir::tryAttributeGet( const HtmlString &dir_set_name, Vector_XDirAttributeSPtr &xDir_result_attribute_value ) const {
	auto mapIterator = attributesMap->begin( );
	auto mapEnd = attributesMap->end( );
	for( ; mapIterator != mapEnd; ++mapEnd )
		if( HtmlStringTools::equRemoveSpaceOverHtmlString( mapIterator->first, dir_set_name ) ) { // 存在
			xDir_result_attribute_value = *mapIterator->second;
			return XDir_Attribute_Status::Get_Array;
		}

	return None;
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
	size_t parseCount = save_vector_xdirattribute_s->size( );
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

	return save_vector_xdirattribute_s->size( ) - parseCount;
}
XDir::XDir( const HtmlString &param ) {
	attributesMap = std::make_shared< UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV >( );
	size_t length = param.length( );
	if( length == 0 )
		return;
	auto data = param.data( );
	HtmlChar value; // 当前校验值
	HtmlChar *buff = new HtmlChar[ length + 1 ]; // 缓存指针
	size_t buffIndex = 0; // 缓存下标
	Vector_XDirAttributeSPtr_Shared attributesResult( std::make_shared< Vector_XDirAttributeSPtr >( ) ); // 存储解析后的属性
	for( size_t index = 0; index < length; ++index ) {
		value = data[ index ];
		if( value == charValue::leftSquareBracket ) { // 找到 ]
			HtmlString xdirName( buff, buffIndex );
			namesList.emplace_back( xdirName );
			buffIndex = 0;
			++index;
			for( ; index < length; ++index ) {
				value = data[ index ];
				if( value == charValue::rightSquareBracket ) { // 找到 ]
					if( buffIndex > 0 ) {
						// 分解属性
						attributesResult->clear( );
						auto projectCount = parseXDirAttributes( buff, buffIndex, attributesResult );
						if( projectCount )
							for( auto &xdir : *attributesResult )
								insertXDirAttributeInfo( xdirName, xdir );
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


bool equNameAttribute( const UMap_HtmlStringK_HtmlStringV_Shared &pairs, const HtmlString &node_name, const HtmlString &first, Vector_XDirAttributeSPtr_Shared &dir_attributes ) {
	if( !HtmlStringTools::equRemoveSpaceOverHtmlString( node_name, first ) )
		return false;
	auto dirAttributes = dir_attributes.get( ); // 节点的所有属性
	if( dirAttributes->size( ) == 0 ) // 没有需要匹配的属性
		return true;
	if( pairs->size( ) == 0 )
		return false;

	for( auto &value : *dirAttributes ) {
		auto dirAttributeName = *value.get( )->getName( );
		auto iterator = pairs->begin( );
		auto end = pairs->end( );
		do {
			auto &className = iterator->first;
			if( HtmlStringTools::equRemoveSpaceOverHtmlString( className, dirAttributeName ) ) {
				const auto &classValue = iterator->second;
				auto name = std::make_shared< HtmlString >( className );
				// todo 
				auto attributeValues = XDirAttribute::converXDirAttributeValues( classValue.data( ), classValue.size( ), name );

				if( value->isIncludeOtherXDirAttributes( *attributeValues ) )
					return true;
			}
			++iterator;
			if( iterator == end ) // 节点无法满足匹配需求（没有属性）
				return false;;
		} while( true );
	}
	return false;
}


bool XDir::hasAttribute( const UMap_HtmlStringK_HtmlStringV_Shared &attribute, const HtmlString &nodeName ) {
	auto iterator = attributesMap->begin( );
	auto end = attributesMap->end( );
	if( iterator == end )
		return true;
	do {
		if( equNameAttribute( attribute, nodeName, iterator->first, iterator->second ) )
			return true;
		++iterator;
		if( iterator == end )
			break;
	} while( true );
	return false;
}
bool tryGet( const UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV_Shared &pairs, const HtmlString &name, Vector_HtmlStringSPtr &vector ) {
	auto oldSize = vector.size( );
	auto mapIterator = pairs->begin( );
	auto mapEnd = pairs->end( );
	for( ; mapIterator != mapEnd; ++mapIterator ) {
		if( HtmlStringTools::equRemoveSpaceOverHtmlString( mapIterator->first, name ) ) {
			auto &vectorSPtr = mapIterator->second;
			auto iterator = vectorSPtr->begin( );
			auto end = vectorSPtr->end( );
			for( ; iterator != end; ++iterator ) {
				auto resultVectorIterator = vector.begin( );
				auto resultVectorEnd = vector.end( );
				HtmlString serializeHtmlString = iterator->get( )->serializeToHtmlString( );
				for( ; resultVectorIterator != resultVectorEnd; ++resultVectorIterator )
					if( HtmlStringTools::equRemoveSpaceOverHtmlString( *resultVectorIterator->get( ), serializeHtmlString ) )
						break;
				if( resultVectorIterator == resultVectorEnd )
					vector.emplace_back( std::make_shared< HtmlString >( serializeHtmlString ) );
			}
		}
	}
	if( vector.size( ) - oldSize > 0 )
		return true;
	return false;
}

inline bool jion( const Vector_HtmlStringSPtr_Shared &html_string_s_ptr, HtmlString &result, const HtmlString &joint ) {
	auto iterator = html_string_s_ptr->begin( );
	auto end = html_string_s_ptr->end( );
	if( iterator == end )
		return false;

	do {
		result.append( *iterator->get( ) );
		++iterator;
		if( iterator == end )
			break;
		result.append( joint );
	} while( true );
	return true;
}
/// <summary>
/// 从 x_dir_attribute_s 序列化字符串到 result_htmlString
/// </summary>
/// <param name="x_dir_attribute_s">序列数据对象列表</param>
/// <param name="result">返回属性名称与值的键值对列表</param>
/// <param name="joint">拼接对象</param>
/// <returns>序列化个数</returns>
inline size_t Vector_XDirAttributeSPtrToHtmlString( Vector_XDirAttributeSPtr &x_dir_attribute_s,
	std::unordered_map< HtmlString, HtmlString > &result,
	const HtmlString &joint ) {
	auto iterator = x_dir_attribute_s.begin( );
	auto end = x_dir_attribute_s.end( );
	if( iterator == end )
		return 0;
	size_t count = 1;
	HtmlString resultNameHtmlString;
	HtmlString resultValueHtmlString;
	HtmlString doubleQuotation( 1, charValue::doubleQuotation );
	do {
		resultNameHtmlString = *iterator->get( )->getName( );
		const auto &ptr = iterator->get( )->getValues( );

		HtmlString subCallResult;

		jion( ptr, subCallResult, joint );
		if( ptr->size( ) > 1 )
			subCallResult = doubleQuotation + subCallResult + doubleQuotation;
		result.emplace( resultNameHtmlString, subCallResult );
		++iterator;
		if( iterator == end )
			break;
		++count;
	} while( true );

	return count;
}


HtmlString XDir::getXDirName( ) const {
	auto nameListIterator = namesList.begin( );
	auto nameListEnd = namesList.end( );
	if( nameListIterator == nameListEnd )
		return HtmlString( );
	HtmlString result;
	HtmlString at( 1, charValue::at );
	HtmlString equ( 1, charValue::equ );
	HtmlString spaceValue( 2, charValue::doubleQuotation );
	HtmlString space( 1, charValue::space );
	HtmlString leftSquareBracket( 1, charValue::leftSquareBracket );
	HtmlString rightSquareBracket( 1, charValue::rightSquareBracket );

	do {
		auto &name = *nameListIterator;
		Vector_XDirAttributeSPtr xDirResultAttributeValues;
		result.append( name );
		if( tryAttributeGet( name, xDirResultAttributeValues ) == Get_Array ) {
			std::unordered_map< HtmlString, HtmlString > resultMap;
			auto serializeCount = Vector_XDirAttributeSPtrToHtmlString( xDirResultAttributeValues, resultMap, space );
			result.append( leftSquareBracket );
			if( serializeCount > 0 ) {
				result.append( at );
				auto iterator = resultMap.begin( );
				if( serializeCount > 1 ) {
					auto end = resultMap.end( );
					do {
						result.append( iterator->first );
						result.append( equ );
						result.append( iterator->second );
						++iterator;
						if( iterator == end )
							break;
					} while( true );
				} else {
					result.append( iterator->first );
					result.append( equ );
					result.append( iterator->second );
				}
			}
			result.append( rightSquareBracket );
		}
		++nameListIterator;
		if( nameListIterator == nameListEnd )
			break;
		result.append( space );
	} while( true );

	return result;
}

HtmlString XDir::getDirName( ) const {
	HtmlString result;
	for( auto &name : namesList )
		result.append( name );
	return result;
}
