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
		if( value == charValue::leftSquareBracket ) { // 找到 [
			HtmlString xdirName;
			if( buffIndex != 0 ) { // 如果缓冲存在数据，则转换为名称
				xdirName = HtmlString( buff, buffIndex );
				namesList.emplace_back( xdirName );
				buffIndex = 0;
			}
			++index;
			for( ; index < length; ++index ) {
				value = data[ index ];
				if( value == charValue::rightSquareBracket ) { // 找到 ]
					if( buffIndex > 0 ) {
						// 分解属性
						attributesResult->clear( );
						auto projectCount = XDirAttribute::parseXDirAttributes( buff, buffIndex, attributesResult );
						if( projectCount )
							for( auto &xdir : *attributesResult )
								insertXDirAttributeInfo( xdirName, xdir );
						buffIndex = 0;
					}
					break;
				} else if( HtmlStringTools::isQuotation( value ) ) { // 引号，则扫描后续引号，并且填充缓存
					size_t getQuotationEnd;
					if( HtmlStringTools::jumpQuotation( data, length, index, getQuotationEnd ) ) {
						buff[ buffIndex ] = value;
						++buffIndex;
						for( ++index; index < getQuotationEnd; ++index, ++buffIndex )
							buff[ buffIndex ] = data[ index ];
						buff[ buffIndex ] = data[ index ];
						++buffIndex;
					} else {// 无法匹配引号，则直接配置结束索引
						index = length;
						buffIndex = 0;
					}
				} else {
					buff[ buffIndex ] = value;
					++buffIndex;
				}
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
XDir::XDir( const cylHtmlTools::HtmlString &name, const XDirAttribute_Shared &xdiattrbute_sptr ) {
	namesList.emplace_back( name );
	attributesMap = std::make_shared< UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV >( );
	if( xdiattrbute_sptr ) {
		auto dirAttributes = std::make_shared< Vector_XDirAttributeSPtr >( );
		dirAttributes->emplace_back( xdiattrbute_sptr );
		attributesMap->emplace( name, dirAttributes );
	}
}
XDir::XDir( const cylHtmlTools::HtmlString &name, const XDirAttribute &xdiattrbute ) : XDir( name, std::make_shared< XDirAttribute >( xdiattrbute ) ) {
}
XDir::XDir( const cylHtmlTools::HtmlString &name, const Vector_XDirAttributeSPtr_Shared &vector_xdiattrbutesptr_sptr ) {
	namesList.emplace_back( name );
	attributesMap = std::make_shared< UMap_HtmlStringK_VectorSPtr_XDirAttributeSPtrV >( );
	if( vector_xdiattrbutesptr_sptr->size( ) > 0 )
		attributesMap->emplace( name, vector_xdiattrbutesptr_sptr );
}
XDir::XDir( const cylHtmlTools::HtmlString &name, const Vector_XDirAttributeSPtr &vector_xdiattrbutesptr ): XDir( name, std::make_shared< Vector_XDirAttributeSPtr >( vector_xdiattrbutesptr ) ) {

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
				if( value->isOtherXDirAttributeValuesIncludeThisXDirAttributeValues( *attributeValues ) )
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
inline size_t Vector_XDirAttributeSPtrToHtmlString( Vector_XDirAttributeSPtr &x_dir_attribute_s, std::unordered_map< HtmlString, HtmlString > &result, const HtmlString &joint ) {
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
						result = result + charValue::space + charValue::at;
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

bool XDir::isLeftXDirIncludeRightXDir( const Vector_XDirAttributeSPtr &left_xdirattributeSPtr_vector, Vector_XDirAttributeSPtr &right_xdirattributeSPtr_vector ) {
	if( left_xdirattributeSPtr_vector.size( ) == 0 )
		return true;
	if( right_xdirattributeSPtr_vector.size( ) == 0 )
		return false;
	auto rightIterator = right_xdirattributeSPtr_vector.begin( );
	auto rightEnd = right_xdirattributeSPtr_vector.end( );
	do {
		auto leftIterator = left_xdirattributeSPtr_vector.begin( );
		auto leftEnd = left_xdirattributeSPtr_vector.end( );
		for( ; leftIterator != leftEnd; ++leftIterator ) // 查找所有左操作元素，检测是否存在匹配的属性
			if( HtmlStringTools::equRemoveSpaceOverHtmlString( leftIterator->get( )->getName( ).get( ), rightIterator->get( )->getName( ).get( ) ) && XDirAttribute::isLeftXDirAttributeValuesIncludeRightXDirAttributeValues( *leftIterator->get( )->getValues( ), *rightIterator->get( )->getValues( ) ) )
				break;
		if( leftIterator == leftEnd )
			break;
		++rightIterator;
		if( rightIterator == rightEnd )
			break;
	} while( true );
	if( rightIterator == rightEnd )
		return true;
	return false;
}

bool XDir::isLeftXDirIncludeRightXDir( const XDir *left_xdir, const XDir *right_xdir ) {

	size_t rightNameSize = right_xdir->namesList.size( );
	if( rightNameSize == 0 )
		return true; // 空集允许被任意集合包含
	size_t leftNameSize = left_xdir->namesList.size( );
	if( leftNameSize == 0 )
		return false; // 空集不包含任何非空子集

	/// 遍历右操作对象所有的元素
	auto rightIterator = right_xdir->namesList.begin( ); // 右操作对象元素遍历下标
	auto rightEnd = right_xdir->namesList.end( );  // 右操作对象元素结束下标
	do {
		/// 遍历左操作对象所有的元素
		auto leftIterator = left_xdir->namesList.begin( ); // 左操作对象元素遍历下标
		auto leftEnd = left_xdir->namesList.end( );  // 左操作对象元素结束下标
		do {
			if( HtmlStringTools::equRemoveSpaceOverHtmlString( *leftIterator, *rightIterator ) )
				break;
			++leftIterator;
			if( leftEnd == leftIterator )
				break;
		} while( true );
		if( leftEnd == leftIterator )
			return false; // 找不到匹配的名称
		Vector_XDirAttributeSPtr rightValues;
		if( right_xdir->tryAttributeGet( *rightIterator, rightValues ) != None ) {
			Vector_XDirAttributeSPtr leftValues;
			if( left_xdir->tryAttributeGet( *leftIterator, leftValues ) == None ) // 没有元素
				return false; // 没有属性
			if( !isLeftXDirIncludeRightXDir( leftValues, rightValues ) ) // 不包含则退出
				break;
		}
		++rightIterator;
		if( rightEnd == rightIterator )
			break;
	} while( true );
	if( rightEnd == rightIterator )
		return true;
	return false;
}


HtmlString XDir::getDirName( ) const {
	HtmlString result;
	for( auto &name : namesList )
		result.append( name );
	return result;
}
