#include "XDirAttribute.h"

#include "../../../htmlString/HtmlStringTools.h"
using namespace cylHtmlTools;
XDirAttribute::XDirAttribute( const HtmlString_Shared &in_name ) {
	name = in_name;
	values = std::make_shared< Vector_HtmlStringSPtr >( );
}
XDirAttribute::XDirAttribute( const HtmlString &in_name ) : XDirAttribute( std::make_shared< HtmlString >( in_name ) ) {
}
XDirAttribute::XDirAttribute( const HtmlString_Shared &in_name, const Vector_HtmlStringSPtr_Shared &in_values ) {
	name = in_name;
	if( in_values ) // 防止第二参数为 nullptr
		values = in_values;
	else
		values = std::make_shared< Vector_HtmlStringSPtr >( );
}
XDirAttribute::XDirAttribute( const HtmlString &in_name, const Vector_HtmlStringSPtr &in_values ): XDirAttribute( std::make_shared< HtmlString >( in_name ), std::make_shared< Vector_HtmlStringSPtr >( in_values ) ) {

}
XDirAttribute::XDirAttribute( const HtmlString &in_name, const std::vector< HtmlString > &in_values ) {
	name = std::make_shared< HtmlString >( in_name );
	values = std::make_shared< Vector_HtmlStringSPtr >( );
	for( auto &value : in_values )
		values->emplace_back( std::make_shared< HtmlString >( value ) );
}

XDirAttribute::XDirAttribute( const HtmlChar *name_buff, const size_t name_buff_size, const HtmlChar *values_buff, const size_t values_size ) {
	name = converXDirAttributeName( name_buff, name_buff_size );
	values = converXDirAttributeValues( values_buff, values_size, name );
}
XDirAttribute::~XDirAttribute( ) {
}

HtmlString XDirAttribute::serializeToHtmlString( ) const {
	HtmlString result( 1, charValue::at );
	result.append( *name ).append( HtmlString( 1, charValue::equ ) );
	auto iterator = values->begin( );
	auto end = values->end( );
	if( iterator == end )
		return result;
	HtmlString doubleQuoation( 1, charValue::doubleQuotation );
	HtmlString space( 1, charValue::space );
	result.append( doubleQuoation );
	do {
		result.append( *iterator->get( ) );
		if( iterator == end )
			break;
		result.append( space );
		++iterator;
	} while( true );

	result.append( doubleQuoation );
	return result;
}
Vector_HtmlStringSPtr_Shared XDirAttribute::normalValues( const HtmlString_Shared &attribute_name, const Vector_HtmlStringSPtr_Shared &xdir_attribute_values ) {
	Vector_HtmlStringSPtr_Shared result( std::make_shared< Vector_HtmlStringSPtr >( ) );
	for( auto &htmlSPtr : *xdir_attribute_values ) {
		Vector_HtmlStringSPtr_Shared attributeValues = converXDirAttributeValues( htmlSPtr->c_str( ), htmlSPtr->size( ), attribute_name );
		if( attributeValues )
			for( auto &valueSPtr : *attributeValues )
				result->emplace_back( valueSPtr );
	}
	if( result->size( ) == 0 )
		return nullptr;
	return result;
}

HtmlString_Shared XDirAttribute::converXDirAttributeName( const HtmlChar *buff, const size_t buff_size ) {
	return htmlCharBuffConverToName( buff, buff_size );
}

Vector_HtmlStringSPtr_Shared XDirAttribute::converXDirAttributeValues( const HtmlChar *buff, const size_t buff_size, const HtmlString_Shared &name ) {
	// 检测是否存在等号
	size_t index = 0;
	if( HtmlStringTools::findNextHtmlCharPotion( buff, buff_size, charValue::equ, index ) ) {
		return converXDirAttributeValues( buff + index + 1, buff_size - index - 1, name );
	} else if( name && !name->empty( ) && isSplitAttributeName( *name ) ) {
		auto resultVectorSPtr = std::make_shared< Vector_HtmlStringSPtr >( );
		auto htmlStringSPtrShared = htmlCharBuffConverToValues( buff, buff_size );
		if( htmlStringSPtrShared )
			for( auto &value : *htmlStringSPtrShared ) {
				HtmlStringTools::removeBothSpace( *value ); // 移除两侧空格
				if( value->empty( ) ) // 跳过空字符串
					continue;
				std::vector< HtmlString > split = HtmlStringTools::split( *value, charValue::space ); // 空格切分
				for( auto &subStr : split )
					if( !subStr.empty( ) ) // 非空字符串加入到返回列表当中
						resultVectorSPtr->emplace_back( std::make_shared< HtmlString >( subStr ) ); // 添加切分后的字符串
			}
		return resultVectorSPtr;
	} else
		return htmlCharBuffConverToValues( buff, buff_size );
	return nullptr;
}
int32_t XDirAttribute::appendAttribute( Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s, const HtmlString_Shared &name_shared ) {
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

int32_t XDirAttribute::appendAttribute( Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s, const HtmlString_Shared &name_shared, const Vector_HtmlStringSPtr_Shared &values_vector_shared ) {
	if( values_vector_shared ) {
		auto &name = *name_shared;
		if( isSplitAttributeName( name ) ) {
			std::vector< HtmlString > splitOverLastStringList;
			for( auto &strSPtr : *values_vector_shared ) {
				auto split = HtmlStringTools::split( *strSPtr, charValue::space );
				for( auto &subStr : split )
					splitOverLastStringList.emplace_back( subStr );
			}
			auto iterator = save_vector_xdirattribute_s->begin( );
			auto end = save_vector_xdirattribute_s->end( );
			for( ; iterator != end; ++iterator ) {
				auto &xdirAttribute = **iterator;
				// 存在属性名称相等则存储所有值
				if( HtmlStringTools::equRemoveSpaceOverHtmlString( *xdirAttribute.getName( ), name ) ) {
					// 遍历值列表
					auto srcValuesVectorIterator = splitOverLastStringList.begin( );
					auto srcValuesVectorEnd = splitOverLastStringList.end( );
					// 获取已经存在的库遍历对象
					auto saveValuesShared = xdirAttribute.getValues( );
					do {
						auto saveValueIterator = saveValuesShared->begin( );
						auto saveValueEnd = saveValuesShared->end( );
						// 查找是否存在相等的值
						for( ; saveValueIterator != saveValueEnd; ++saveValueIterator )
							if( HtmlStringTools::equRemoveSpaceOverHtmlString( *saveValueIterator->get( ), *srcValuesVectorIterator ) )
								break;
						if( saveValueIterator == saveValueEnd )// 如果没有找到匹配项，则加入这个值
							// class 属性名称的值，需要切分
							saveValuesShared->emplace_back( std::make_shared< HtmlString >( *srcValuesVectorIterator ) );
						++srcValuesVectorIterator; // 遍历下一个
						if( srcValuesVectorIterator == srcValuesVectorEnd ) // 全部值已经遍历完毕
							break;
					} while( true );
					break; // 已经处理了该属性
				}
			}
			if( iterator == end ) // 没有匹配的属性，则把属性加入到当前属性列表当中
				save_vector_xdirattribute_s->emplace_back( std::make_shared< XDirAttribute >( *name_shared, splitOverLastStringList ) );
			return 0;
		} else {
			auto iterator = save_vector_xdirattribute_s->begin( );
			auto end = save_vector_xdirattribute_s->end( );
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
	}
	return appendAttribute( save_vector_xdirattribute_s, name_shared );
}
Vector_HtmlStringSPtr_Shared XDirAttribute::htmlCharBuffConverToValues( const HtmlChar *conver_buff, const size_t conver_buff_size ) {

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
			if( HtmlStringTools::jumpQuotation( conver_buff, conver_buff_size, index, endIndex ) )
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
HtmlString_Shared XDirAttribute::htmlCharBuffConverToName( const HtmlChar *conver_buff, const size_t conver_buff_size ) {
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
			if( HtmlStringTools::jumpQuotation( conver_buff, conver_buff_size, index, endIndex ) )
				result->append( HtmlString( conver_buff + index + 1, endIndex - index - 1 ) );
			else
				break;
			index = endIndex + 1;
		} else if( HtmlStringTools::isSpace( currentCharValue ) ) { // 如果是空字符，则参考 buff 是否有缓存
			if( buffIndex == 0 )
				continue;
			result->append( HtmlString( buff, buffIndex ) );
			buffIndex = 0;
		} else if( currentCharValue == charValue::equ ) { // 如果是等号
			break;
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
size_t XDirAttribute::parseXDirAttributes( const HtmlChar *buff, const size_t buff_size, Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s ) {
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
				appendAttribute( save_vector_xdirattribute_s, nameHtmlShared, values );
				break;
			}
		}
	}
	return save_vector_xdirattribute_s->size( ) - parseCount;
}
bool XDirAttribute::isSplitAttributeName( const HtmlString &name ) {
	return HtmlStringTools::equRemoveSpaceOverHtmlString( name, L"class" ) || HtmlStringTools::equRemoveSpaceOverHtmlString( name, L"id" );

}
