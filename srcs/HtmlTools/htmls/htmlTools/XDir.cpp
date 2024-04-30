#include "XDir.h"

#include <iostream>

#include "../../wstr/WStrTools.h"
using namespace htmlTools;

static bool insertOrAssign( HtmlStringMapToVectorHtmlStrSPtr &mapObj, HtmlString &key, HtmlString &value ) {
	auto iterator = mapObj.begin( );
	auto end = mapObj.end( );
	for( ; iterator != end; ++iterator )
		if( iterator->first == key ) {
			iterator->second->emplace_back( std::make_shared< HtmlString >( value ) );
			return true;
		}
	auto list( std::make_shared< Vector_HtmlStringSPtr >( ) );
	list->emplace_back( std::make_shared< HtmlString >( value ) );
	mapObj.insert( std::make_pair( key, list ) );
	return true;
}
static bool tryGet( HtmlStringMapToVectorHtmlStrSPtr &mapObj, HtmlString &key, Vector_HtmlStringSPtr &value ) {
	auto iterator = mapObj.begin( );
	auto end = mapObj.end( );
	for( ; iterator != end; ++iterator )
		if( iterator->first == key ) {
			auto sharedPtrs = iterator->second;
			for( auto &ptr : *sharedPtrs )
				value.emplace_back( std::make_shared< HtmlString >( *ptr ) );
			return true;
		}
	return false;
}

XDir::XDir( const htmlTools::HtmlString &param ) {
	size_t length = param.length( );
	if( length == 0 )
		return;
	htmlTools::HtmlChar value;
	std::vector< htmlTools::HtmlChar > buff;
	for( size_t index = 0; index < length; ++index ) {
		value = param[ index ];
		if( value == L'[' ) { // 找到 ]
			HtmlString name( buff.data( ), buff.size( ) );
			namesList.emplace_back( name );
			buff.clear( );
			++index;
			for( ; index < length; ++index ) {
				value = param[ index ];
				if( value == L']' ) { // 找到 ]
					if( buff.size( ) > 0 ) {
						HtmlString mapValue( buff.data( ), buff.size( ) );
						insertOrAssign( attributesList, name, mapValue );
						buff.clear( );
					}
					break;
				}
				if( value == charValue::doubleQuotation ) {
					buff.emplace_back( value );
					++index;
					for( ; index < length; ++index ) {
						value = param[ index ];
						buff.emplace_back( value );
						if( value == charValue::doubleQuotation )
							break;
					}
					continue;
				}
				if( value == charValue::singleQuotation ) {
					buff.emplace_back( value );
					++index;
					for( ; index < length; ++index ) {
						value = param[ index ];
						buff.emplace_back( value );
						if( param[ index ] == charValue::singleQuotation )
							break;
					}
					continue;
				}
				buff.emplace_back( value );
			}
			continue;
		}
		buff.emplace_back( value );
	}
	if( buff.size( ) > 0 ) {
		HtmlString name( buff.data( ), buff.size( ) );
		namesList.emplace_back( name );
	}
}
XDir::~XDir( ) {
}
bool XDir::hasName( const HtmlString &dir_name ) {
	for( auto &name : namesList )
		if( dir_name == name )
			return true;
	return false;
}

void insertOrAssign( std::unordered_map< HtmlString, HtmlStringPairUnorderMap > &mapObj, HtmlString &key, std::pair< HtmlString, HtmlString > &value ) {
	auto iterator = mapObj.begin( );
	auto end = mapObj.end( );
	for( ; iterator != end; ++end )
		if( iterator->first == key ) {
			auto &map = iterator->second;
			map.insert_or_assign( value.first, value.second );
			return;
		}
	std::unordered_map< std::wstring, std::wstring > unorderedMap;
	unorderedMap.insert_or_assign( value.first, value.second );
	mapObj.insert_or_assign( key, unorderedMap );

}

/// <summary>
/// 生成属性映射列表，而不是路径映射列表
/// </summary>
/// <param name="src">路径映射列表</param>
/// <param name="target">属性映射列表</param>
/// <returns>列表个数</returns>
static size_t initAttributesMap(
	HtmlStringMapToVectorHtmlStrSPtr &src,
	std::unordered_map< HtmlString, HtmlStringPairUnorderMap > &target ) {
	auto iterator = src.begin( );
	auto end = src.end( );
	size_t equIndex, index;
	std::vector< HtmlChar > bufff;
	for( ; iterator != end; ++iterator ) { // 遍历属性列表
		auto second = iterator->second;
		for( auto &attributeIterator : *second ) {
			size_t length = attributeIterator->length( );
			auto dirKey = iterator->first;
			for( index = 0; index < length; ++index ) {
				auto valueChar = attributeIterator->at( index );
				if( valueChar == charValue::at ) { // 分解属性
					for( equIndex = index + 1; equIndex < length; ++equIndex ) {
						valueChar = attributeIterator->at( equIndex );
						if( valueChar == charValue::equ ) {
							for( index = equIndex + 1; index < length; ++index )
								if( !HtmlStringTools::isJumpSpace( attributeIterator->at( index ) ) )
									break;
							break;
						}
						bufff.emplace_back( valueChar );
					}

					size_t buffSize = bufff.size( );
					if( buffSize ) {
						auto key( HtmlString( bufff.data( ), buffSize ) );
						bufff.clear( );
						for( ; index < length; ++index ) {
							valueChar = attributeIterator->at( index );
							if( HtmlStringTools::isJumpSpace( valueChar ) )
								break;
							bufff.emplace_back( valueChar );
							if( valueChar == charValue::doubleQuotation ) {
								for( index = index + 1; index < length; ++index ) {
									valueChar = attributeIterator->at( index );
									bufff.emplace_back( valueChar );
									if( valueChar == charValue::doubleQuotation )
										break;
								}
								continue;
							}
							if( valueChar == charValue::singleQuotation ) {
								for( index = index + 1; index < length; ++index ) {
									valueChar = attributeIterator->at( index );
									bufff.emplace_back( valueChar );
									if( valueChar == charValue::singleQuotation )
										break;
								}
								continue;
							}

						}
						buffSize = bufff.size( );
						auto value( HtmlString( bufff.data( ), buffSize ) );
						bufff.clear( );
						insertOrAssign( target, dirKey, std::make_pair( key, value ) );
					}
				}

			}
		}
	}
	return target.size( );
}

bool compAttributesKey( HtmlString left, HtmlString right ) {
	size_t leftLen = left.length( );
	size_t rightLen = right.length( );
	size_t index = 0, buffIndex = 0;
	for( ; index < leftLen; ++index )
		if( !HtmlStringTools::isJumpSpace( left[ index ] ) ) {
			buffIndex = leftLen - 1;
			leftLen -= index;
			do {
				if( !HtmlStringTools::isJumpSpace( left[ buffIndex ] ) )
					break;
				if( buffIndex == 0 )
					break;
				--buffIndex;
			} while( true );
			left = left.substr( index, leftLen * 2 - index - buffIndex - 1 );
			leftLen = left.length( );
			break;
		}

	for( index = 0; index < rightLen; ++index )
		if( !HtmlStringTools::isJumpSpace( right[ index ] ) ) {
			buffIndex = rightLen - 1;
			leftLen -= index;
			do {
				if( !HtmlStringTools::isJumpSpace( right[ buffIndex ] ) )
					break;
				if( buffIndex == 0 )
					break;
				--buffIndex;
			} while( true );
			right = right.substr( index, rightLen * 2 - index - buffIndex - 1 );
			rightLen = right.length( );
			break;
		}

	if( leftLen == rightLen ) {
		for( index = 0; index < leftLen; ++index ) {
			auto leftChar = left[ index ];
			auto rightChar = right[ index ];
			bool leftIsEquQuotation = rightChar == charValue::doubleQuotation || rightChar == charValue::singleQuotation;
			if( leftIsEquQuotation ) { // 跳过引号
				bool rightIsEquQuotation = leftChar == charValue::doubleQuotation || leftChar == charValue::singleQuotation;
				if( !rightIsEquQuotation )
					return false;
				continue;
			}
			if( leftChar != rightChar )
				return false;
		}
		return true;
	}

	return left == right;
}
bool XDir::hasAttribute( const HtmlStringPairUnorderMap_Shared &attribute, const HtmlString &nodeName ) {
	if( attribute->size( ) > 0 ) {
		if( attributesMap.size( ) == 0 &&
			initAttributesMap( attributesList, attributesMap ) == 0 ) // 当前 xpath 不要求存在属性
			return true;
		// 检查属性
		HtmlStringPairUnorderMap unorderedMap = attributesMap.at( nodeName );
		if( unorderedMap.size( ) == 0 )
			return false;
		auto begin = unorderedMap.begin( );
		auto dirEnd = unorderedMap.end( );
		for( ; begin != dirEnd; ++begin ) {
			auto iterator = attribute->begin( );
			auto end = attribute->end( );
			for( ; iterator != end; ++iterator ) {
				if( compAttributesKey( iterator->first, begin->first ) ) {
					if( compAttributesKey( iterator->second, begin->second ) )
						return true;
				}
			}
		}
		return false;
	}

	return true;
}
HtmlString XDir::getDirName( ) {
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
