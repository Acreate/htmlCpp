#include "XDir.h"

#include <iostream>

#include "../../../htmlString/HtmlStringTools.h"

#include "../XDirAttribute/XDirAttribute.h"
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
	for( ; mapIterator != mapEnd; ++mapIterator )
		if( HtmlStringTools::equRemoveSpaceOverHtmlString( mapIterator->first, dir_set_name ) ) { // 存在
			xDir_result_attribute_value = *mapIterator->second;
			return XDir_Attribute_Status::Get_Array;
		}

	return None;
}

/// <summary>
/// 追加一个属性
/// </summary>
/// <param name="save_vector_xdirattribute_s">属性列表</param>
/// <param name="name_shared">属性名称</param>
/// <returns>失败返回非 0</returns>
inline int32_t appendAttribute( Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s, const HtmlString_Shared &name_shared ) {
	if( name_shared ) {
		auto iterator = save_vector_xdirattribute_s->begin( );
		auto end = save_vector_xdirattribute_s->end( );
		auto &name = *name_shared;
		for( ; iterator != end; ++iterator )
			// 存在属性名称相等则存储所有值
			if( HtmlStringTools::equRemoveSpaceOverHtmlString( *iterator->get( )->getName( ), name ) )
				break; // 已经处理了该属性
		if( iterator == end ) // 没有匹配的属性，则把属性加入到当前属性列表当中
			save_vector_xdirattribute_s->emplace_back( std::make_shared< XDirAttribute >( name_shared, nullptr ) );
		return 0;
	}
	return 1;
}
/// <summary>
/// 追加一个属性
/// </summary>
/// <param name="save_vector_xdirattribute_s">属性列表</param>
/// <param name="name_shared">属性名称</param>
/// <param name="values_vector_shared">属性值列表</param>
/// <returns>失败返回非 0</returns>
inline int32_t appendAttribute( Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s, const HtmlString_Shared &name_shared, const Vector_HtmlStringSPtr_Shared &values_vector_shared ) {
	if( values_vector_shared ) {
		auto iterator = save_vector_xdirattribute_s->begin( );
		auto end = save_vector_xdirattribute_s->end( );
		auto &name = *name_shared;
		for( ; iterator != end; ++iterator ) {
			auto &xdirAttribute = **iterator;
			// 存在属性名称相等则存储所有值
			if( HtmlStringTools::equRemoveSpaceOverHtmlString( *xdirAttribute.getName( ), name ) ) {
				// 遍历值列表
				auto srcValuesVectorIterator = values_vector_shared->begin( );
				auto srcValuesVectorEnd = values_vector_shared->end( );
				// 获取已经存在的库遍历对象
				auto saveValuesShared = xdirAttribute.getValues( );
				do {
					auto saveValueIterator = saveValuesShared->begin( );
					auto saveValueEnd = saveValuesShared->end( );
					// 查找是否存在相等的值
					for( ; saveValueIterator != saveValueEnd; ++saveValueIterator )
						if( HtmlStringTools::equRemoveSpaceOverHtmlString( *saveValueIterator, *srcValuesVectorIterator ) )
							break;
					if( saveValueIterator == saveValueEnd ) // 如果没有找到匹配项，则加入这个值
						saveValuesShared->emplace_back( *srcValuesVectorIterator );
					++srcValuesVectorIterator; // 遍历下一个
					if( srcValuesVectorIterator == srcValuesVectorEnd ) // 全部值已经遍历完毕
						break;
				} while( true );
				break; // 已经处理了该属性
			}
		}
		if( iterator == end ) // 没有匹配的属性，则把属性加入到当前属性列表当中
			save_vector_xdirattribute_s->emplace_back( std::make_shared< XDirAttribute >( name_shared, values_vector_shared ) );
		return 0;
	}
	return appendAttribute( save_vector_xdirattribute_s, name_shared );
}
inline Vector_HtmlStringSPtr_Shared htmlCharBuffConverToValues( const HtmlChar *conver_buff, const size_t conver_buff_size ) {

	size_t index = 0;
	// 跳过开始的 @
	for( ; index < conver_buff_size; ++index )
		if( conver_buff[ index ] != charValue::at )
			break;
	Vector_HtmlStringSPtr_Shared result( std::make_shared< Vector_HtmlStringSPtr >( ) );
	HtmlChar *buff = new HtmlChar[ conver_buff_size ];
	size_t buffIndex = 0;
	for( ; index < conver_buff_size; ++index ) {
		HtmlChar currentCharValue = conver_buff[ index ];
		if( HtmlStringTools::isQuotation( currentCharValue ) ) {
			if( buffIndex > 0 ) { // 优先处理缓冲
				result->emplace_back( std::make_shared< HtmlString >( buff, buffIndex ) );
				buffIndex = 0;
			}
			// 处理引号
			size_t endIndex;
			std::vector< std::pair< size_t, size_t > > getQuotationPositionS;
			if( HtmlStringTools::jumpQuotation( conver_buff, conver_buff_size, index, endIndex, getQuotationPositionS ) )
				result->emplace_back( std::make_shared< HtmlString >( conver_buff + index + 1, endIndex - index - 1 ) );
			else
				break;
			index = endIndex + 1;
		} else if( HtmlStringTools::isSpace( currentCharValue ) ) { // 如果是空字符，则参考 buff 是否有缓存
			if( buffIndex == 0 )
				continue;
			result->emplace_back( std::make_shared< HtmlString >( buff, buffIndex ) );
			buffIndex = 0;
		} else { // 当前字符存储到缓存
			buff[ buffIndex ] = currentCharValue;
			++buffIndex;
		}
	}
	if( buffIndex > 0 ) { // 处理缓冲
		result->emplace_back( std::make_shared< HtmlString >( buff, buffIndex ) );
		return result;
	}
	if( result->size( ) > 0 )
		return result;
	return nullptr;
}
inline HtmlString_Shared htmlCharBuffConverToName( const HtmlChar *conver_buff, const size_t conver_buff_size ) {
	size_t index = 0;
	// 跳过开始的 @
	for( ; index < conver_buff_size; ++index )
		if( conver_buff[ index ] != charValue::at )
			break;
	HtmlString_Shared result( std::make_shared< HtmlString >( ) );
	HtmlChar *buff = new HtmlChar[ conver_buff_size ];
	size_t buffIndex = 0;
	for( ; index < conver_buff_size; ++index ) {
		HtmlChar currentCharValue = conver_buff[ index ];
		if( HtmlStringTools::isQuotation( currentCharValue ) ) {
			if( buffIndex > 0 ) { // 优先处理缓冲
				result->append( HtmlString( buff, buffIndex ) );
				buffIndex = 0;
			}
			// 处理引号
			size_t endIndex;
			std::vector< std::pair< size_t, size_t > > getQuotationPositionS;
			if( HtmlStringTools::jumpQuotation( conver_buff, conver_buff_size, index, endIndex, getQuotationPositionS ) )
				result->append( HtmlString( conver_buff + index + 1, endIndex - index - 1 ) );
			else
				break;
			index = endIndex + 1;
		} else if( HtmlStringTools::isSpace( currentCharValue ) ) { // 如果是空字符，则参考 buff 是否有缓存
			if( buffIndex == 0 )
				continue;
			result->append( HtmlString( buff, buffIndex ) );
			buffIndex = 0;
		} else { // 当前字符存储到缓存
			buff[ buffIndex ] = currentCharValue;
			++buffIndex;
		}
	}
	if( buffIndex > 0 ) { // 处理缓冲
		result->append( HtmlString( buff, buffIndex ) );
		return result;
	}
	if( result->size( ) > 0 )
		return result;
	return nullptr;
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
	for( ; index < buff_size; ++index ) {
		charValue = buff[ index ];
		if( charValue == charValue::at ) { // 找到了 @ 符号
			++index;
			size_t nextIndex = index, equIndex = nextIndex;
			HtmlString_Shared nameHtmlShared;
			Vector_HtmlStringSPtr_Shared values;
			// 查找下一个 @，判断属性数量
			if( HtmlStringTools::findNextHtmlCharPotion( buff, buff_size, charValue::at, nextIndex ) ) {// 存在 @
				// 开始处理已知 @ 属性( 首次的 @ 下标为 index，其次 @ 下标为 nextIndex)

				// 查找 =，判断是否存在值
				if( HtmlStringTools::findNextHtmlCharPotion( buff, buff_size, charValue::equ, equIndex ) ) {
					nameHtmlShared = htmlCharBuffConverToName( buff + index, equIndex - index );
					equIndex += 1;
					values = htmlCharBuffConverToValues( buff + equIndex, nextIndex - equIndex );
				} else  // 没有值，直接给与 name
					nameHtmlShared = htmlCharBuffConverToName( buff + index, nextIndex - index );
				std::wcout << L"=====================" << std::endl;
				if( nameHtmlShared )
					std::wcout << L"找到了名称 :\"" << *nameHtmlShared << '\"' << std::endl;
				else
					std::wcout << L"名称异常" << std::endl;
				if( values )
					for( auto &str_shared : *values )
						std::wcout << L"找到了值 :\"" << *str_shared << '\"' << std::endl;
				else
					std::wcout << L"值异常" << std::endl;
				std::wcout << L"=====================" << std::endl;
				appendAttribute( save_vector_xdirattribute_s, nameHtmlShared, values );
				// 准备下一次直接进入 @ 流程
				index = nextIndex - 1;
			} else { // 没有下一个 @ 的时候，处理完成后直接退出(单个属性)
				// 查找 =，判断是否存在值
				if( HtmlStringTools::findNextHtmlCharPotion( buff, buff_size, charValue::equ, equIndex ) ) {
					nameHtmlShared = htmlCharBuffConverToName( buff + index, equIndex - index );
					equIndex += 1;
					values = htmlCharBuffConverToValues( buff + equIndex, nextIndex - equIndex );
				} else  // 没有值，直接给与 name
					nameHtmlShared = htmlCharBuffConverToName( buff + index, nextIndex - index );
				std::wcout << L"=====================" << std::endl;
				if( nameHtmlShared )
					std::wcout << L"找到了名称 :\"" << *nameHtmlShared << '\"' << std::endl;
				else
					std::wcout << L"名称异常" << std::endl;

				if( values )
					for( auto &str_shared : *values )
						std::wcout << L"找到了值 :\"" << *str_shared << '\"' << std::endl;
				else
					std::wcout << L"值异常" << std::endl;
				std::wcout << L"=====================" << std::endl;
				appendAttribute( save_vector_xdirattribute_s, nameHtmlShared, values );
				break;
			}
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
	if( namesList.size( ) == 0 )
		return false;
	for( auto &&name : namesList )
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


bool XDir::hasAttribute( const UMap_HtmlStringK_HtmlStringV_Shared &attribute, const HtmlString &nodeName ) const {
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
