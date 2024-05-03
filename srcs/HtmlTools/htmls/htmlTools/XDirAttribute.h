#ifndef XDIRATTRIBUTE_H_H_HEAD__FILE__
#define XDIRATTRIBUTE_H_H_HEAD__FILE__
#pragma once
#include "../../htmlString/HtmlStringTools.h"
#include "../../nameSpace/cylHtmlTools.h"
namespace cylHtmlTools {
	class HTMLTOOLS_EXPORT XDirAttribute {
	public:
		friend class XDir;
	private:
		HtmlString_Shared name;
		Vector_HtmlStringSPtr_Shared values;
	public:
		XDirAttribute( const HtmlString_Shared &in_name, const Vector_HtmlStringSPtr_Shared &in_values );
		XDirAttribute( const HtmlChar *name_buff, const size_t name_buff_size, const HtmlChar *values_buff, const size_t values_size );
		virtual ~XDirAttribute( );
	public: // - 属性
		HtmlString_Shared getName( ) const {
			return name;
		}
		Vector_HtmlStringSPtr_Shared getValues( ) const {
			return values;
		}
		HtmlString serializeToHtmlString( ) const;
		inline operator HtmlString( ) const {
			return serializeToHtmlString( );
		}
	public: // - 操作函数


		/// <summary>
		/// 该对象属性是否与值匹配
		/// </summary>
		/// <param name="other">匹配的值</param>
		/// <returns>存在返回 true</returns>
		bool XDirAttribute::isXDirAttributeIncludeOther( const HtmlString &other ) const {
			size_t thisSize = values->size( );
			if( thisSize < 1 ) // 空集
				return false;
			for( auto &value : *values ) // 存储的值列表存在一个则返回 true
				if( HtmlStringTools::equRemoveSpaceOverHtmlString( *value, other ) )
					return true;
			return false;
		}
		/// <summary>
		/// 该对象是否完全包含列表参数中的所有值
		/// </summary>
		/// <param name="others">列表值</param>
		/// <returns>包含列表当中所有的值时，返回 true</returns>
		bool XDirAttribute::isXDirAttributeIncludesOther( const Vector_HtmlStringSPtr &others ) const {
			size_t thisSize = values->size( ),
				otherSize = others.size( );
			if( thisSize < otherSize ) // 集的元素小于被包含的集
				return false;
			for( auto &other : others ) {
				auto thisValuesIterator = values->begin( );
				auto thisValuesIteratorEnd = values->end( );
				do {
					if( HtmlStringTools::equRemoveSpaceOverHtmlString( *thisValuesIterator->get( ), *other.get( ) ) )
						break;
					++thisValuesIterator;
					if( thisValuesIterator == thisValuesIteratorEnd )
						break;
				} while( true );
				if( thisValuesIterator == thisValuesIteratorEnd )
					return false;
			}
			return true;
		}
		/// <summary>
		/// 该对象是否完全包含列表参数中的所有值
		/// </summary>
		/// <param name="others">列表值</param>
		/// <returns>包含列表当中所有的值时，返回 true</returns>
		bool XDirAttribute::isXDirAttributeIncludesOther( const std::vector< HtmlString > &others ) const {
			size_t thisSize = values->size( ),
				otherSize = others.size( );
			if( thisSize < otherSize ) // 集的元素小于被包含的集
				return false;
			for( auto &other : others ) {
				auto thisValuesIterator = values->begin( );
				auto thisValuesIteratorEnd = values->end( );
				do {
					if( HtmlStringTools::equRemoveSpaceOverHtmlString( *thisValuesIterator->get( ), other ) )
						break;
					++thisValuesIterator;
					if( thisValuesIterator == thisValuesIteratorEnd )
						break;
				} while( true );
				if( thisValuesIterator == thisValuesIteratorEnd )
					return false;
			}
			return true;
		}
		/// <summary>
		/// 该对象属性是否与值匹配
		/// </summary>
		/// <param name="other">匹配的值</param>
		/// <returns>存在返回 true</returns>
		bool XDirAttribute::isIncludeOtherXDirAttribute( const HtmlString &other ) const {
			size_t thisSize = values->size( );
			if( thisSize == 0 ) // 空集
				return true;
			if( thisSize > 1 ) // 超出集合个数
				return false;
			return HtmlStringTools::equRemoveSpaceOverHtmlString( *values->at( 0 ), other ); // 对比元素
		}
		/// <summary>
		/// 列表是否包含本对象的所有值<br/>
		/// 列表可以不存在对象的值，但是对象的所有值必须存在列表当中
		/// </summary>
		/// <param name="others">校验包含的对象</param>
		/// <returns>完全被包含返回 true</returns>
		bool isIncludeOtherXDirAttributes( const std::vector< HtmlString > &others ) const {
			if( this->values->size( ) == 0 ) // 任意集合包含空集
				return true;
			for( auto &value : *values ) {
				auto valuesIterator = others.begin( );
				auto valuesIteratorEnd = others.end( );
				do {
					if( HtmlStringTools::equRemoveSpaceOverHtmlString( *value, *valuesIterator ) )
						break;
					++valuesIterator;
					if( valuesIterator == valuesIteratorEnd )
						break;
				} while( true );
				if( valuesIterator == valuesIteratorEnd )
					return false;
			}
			return true;
		}

		/// <summary>
		/// 列表是否包含本对象的所有值<br/>
		/// 列表可以不存在对象的值，但是对象的所有值必须存在列表当中
		/// </summary>
		/// <param name="others">校验包含的对象</param>
		/// <returns>完全被包含返回 true</returns>
		bool isIncludeOtherXDirAttributes( const std::vector< HtmlString_Shared > &others ) const {
			if( this->values->size( ) == 0 ) // 任意集合包含空集
				return true;
			for( auto &value : *values ) {
				auto valuesIterator = others.begin( );
				auto valuesIteratorEnd = others.end( );
				do {
					if( HtmlStringTools::equRemoveSpaceOverHtmlString( *value, *valuesIterator->get( ) ) )
						break;
					++valuesIterator;
					if( valuesIterator == valuesIteratorEnd )
						break;
				} while( true );
				if( valuesIterator == valuesIteratorEnd )
					return false;
			}
			return true;
		}

		/// <summary>
		/// 列表是否包含本对象的所有值<br/>
		/// 列表可以不存在对象的值，但是对象的所有值必须存在列表当中
		/// </summary>
		/// <param name="others">校验包含的对象</param>
		/// <returns>完全被包含返回 true</returns>
		bool isIncludeOtherXDirAttributes( const XDirAttribute &others ) const {
			return isIncludeOtherXDirAttributes( *others.values );
		}
	public: // - 解析函数
		/// <summary>
		/// 从缓冲当中转换到名称
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <returns>名称对象指针，失败返回 nullptr</returns>
		static HtmlString_Shared converXDirAttributeName( const HtmlChar *buff, const size_t buff_size );
		/// <summary>
		/// 从缓冲当中转换到值
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲大小</param>
		/// <param name="name">属性名称</param>
		/// <returns>值列表对象指针，失败返回 nullptr</returns>
		static Vector_HtmlStringSPtr_Shared converXDirAttributeValues( const HtmlChar *buff, const size_t buff_size, HtmlString_Shared &name );
	};
}

#endif // XDIRATTRIBUTE_H_H_HEAD__FILE__
