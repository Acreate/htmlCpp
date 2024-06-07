#ifndef XDIRATTRIBUTE_H_H_HEAD__FILE__
#define XDIRATTRIBUTE_H_H_HEAD__FILE__
#pragma once
#include <iostream>

#include "../../../htmlString/HtmlStringTools.h"
#include "../../../nameSpace/cylHtmlTools.h"
namespace cylHtmlTools {
	/// <summary>
	/// XDir 属性节点
	/// </summary>
	class HTMLTOOLS_EXPORT XDirAttribute {
	public:
		friend class XDir;
	private:
		HtmlString_Shared name;
		Vector_HtmlStringSPtr_Shared values;
	public:
		XDirAttribute( const HtmlString_Shared &in_name );
		XDirAttribute( const HtmlString &in_name );
		XDirAttribute( const HtmlString_Shared &in_name, const Vector_HtmlStringSPtr_Shared &in_values );
		XDirAttribute( const HtmlString &in_name, const Vector_HtmlStringSPtr &in_values );
		XDirAttribute( const HtmlString &in_name, const std::vector< HtmlString > &in_values );
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
		/// 比较 left_xdirAttribute_vector_ptr 是否包含 right_xdirAttribute_vector_ptr
		/// </summary>
		/// <param name="left_xdirAttribute_vector_ptr">超集</param>
		/// <param name="right_xdirAttribute_vector_ptr">子集</param>
		/// <returns>true 表示 left_xdirAttribute_vector_ptr 是 right_xdirAttribute_vector_ptr 的超集</returns>
		static bool isLeftXDirAttributeValuesIncludeRightXDirAttributeValues( const Vector_HtmlStringSPtr *left_xdirAttribute_vector_ptr, const Vector_HtmlStringSPtr *right_xdirAttribute_vector_ptr ) {
			return isLeftXDirAttributeValuesIncludeRightXDirAttributeValues( *left_xdirAttribute_vector_ptr, *right_xdirAttribute_vector_ptr );
		}
		/// <summary>
		/// 比较 left_xdirAttribute_vector_ptr 是否包含 right_xdirAttribute_vector_ptr
		/// </summary>
		/// <param name="left_xdirAttribute_vector">超集</param>
		/// <param name="right_xdirAttribute_vector">子集</param>
		/// <returns>true 表示 left_xdirAttribute_vector_ptr 是 right_xdirAttribute_vector_ptr 的超集</returns>
		static bool isLeftXDirAttributeValuesIncludeRightXDirAttributeValues( const Vector_HtmlStringSPtr &left_xdirAttribute_vector, const Vector_HtmlStringSPtr &right_xdirAttribute_vector ) {
			size_t rightVectorSize = right_xdirAttribute_vector.size( );
			if( rightVectorSize == 0 )  // 空集是任意集合的子集
				return true;
			size_t leftVectorSize = left_xdirAttribute_vector.size( );
			if( leftVectorSize == 0 ) // 空集不能包含除空集以外的集合
				return false;
			// 获取所有右侧集合的元素
			auto rightIterator = right_xdirAttribute_vector.begin( );
			auto rightEnd = right_xdirAttribute_vector.end( );
			do {
				// 获取所有左侧集合的元素
				auto leftIterator = left_xdirAttribute_vector.begin( );
				auto leftEnd = left_xdirAttribute_vector.end( );
				for( ; leftIterator != leftEnd; ++leftIterator )
					if( HtmlStringTools::equRemoveSpaceOverHtmlString( *leftIterator->get( ), *rightIterator->get( ) ) )
						break;
				if( leftIterator == leftEnd ) // 如果遍历完成后都没有中断，则表示 左侧集合不包含当前 rightIterator 元素
					return false;
				++rightIterator;
				if( rightIterator == rightEnd )
					break;
			} while( true );
			return true;
		}
		/// <summary>
		/// 重新调整值列表
		/// </summary>
		/// <param name="attribute_name">调整的键方案</param>
		/// <param name="xdir_attribute_values">需要被调整的值列表</param>
		/// <returns>完成调整方案的列表</returns>
		static Vector_HtmlStringSPtr_Shared normalValues( const HtmlString_Shared &attribute_name, const Vector_HtmlStringSPtr_Shared &xdir_attribute_values );
	public:
		/// <summary>
		/// 该对象是否完全包含列表参数中的所有值
		/// </summary>
		/// <param name="others">列表值</param>
		/// <returns>包含列表当中所有的值时，返回 true</returns>
		bool isThisXDirAttributeValuesIncludeOtherXDirAttributeValues( const Vector_HtmlStringSPtr &others ) const {
			return this->isLeftXDirAttributeValuesIncludeRightXDirAttributeValues( this->values.get( ), &others );
		}
		/// <summary>
		/// 该对象是否完全包含列表参数中的所有值 values ⊇ others
		/// </summary>
		/// <param name="others">列表值</param>
		/// <returns>包含列表当中所有的值时，返回 true</returns>
		bool isThisXDirAttributeValuesIncludeOtherXDirAttributeValues( const std::vector< HtmlString > &others ) const {
			auto buffRight = std::make_shared< Vector_HtmlStringSPtr >( );
			for( auto &subStr : others )
				buffRight->emplace_back( std::make_shared< HtmlString >( subStr ) );
			return isLeftXDirAttributeValuesIncludeRightXDirAttributeValues( *this->values.get( ), *buffRight );
		}
		/// <summary>
		/// 该对象属性是否包含对应的值 other ∈ values
		/// </summary>
		/// <param name="other">被包含的值</param>
		/// <returns>存在返回 true</returns>
		bool isIncludeOtherXDirAttributeValue( const HtmlString &other ) const {
			size_t thisSize = values->size( );
			if( thisSize == 0 ) // 空集
				return true;
			if( thisSize > 1 ) // 超出集合个数
				return false;
			for( auto &value : *values )
				if( HtmlStringTools::equRemoveSpaceOverHtmlString( *value, other ) )// 对比元素
					return true;
			return false;
		}
		/// <summary>
		/// 列表是否包含本对象的所有值<br/>
		/// 列表可以不存在对象的值，但是对象的所有值必须存在列表当中 values ⊆ others
		/// </summary>
		/// <param name="others">校验包含的对象</param>
		/// <returns>完全被包含返回 true</returns>
		bool isOtherXDirAttributeValuesIncludeThisXDirAttributeValues( const std::vector< HtmlString > &others ) const {
			auto buffLeft = std::make_shared< Vector_HtmlStringSPtr >( );
			for( auto &subStr : others )
				buffLeft->emplace_back( std::make_shared< HtmlString >( subStr ) );
			return isLeftXDirAttributeValuesIncludeRightXDirAttributeValues( *buffLeft, *this->values.get( ) );
		}

		/// <summary>
		/// 列表是否包含本对象的所有值<br/>
		/// 列表可以不存在对象的值，但是对象的所有值必须存在列表当中 values ⊆ others
		/// </summary>
		/// <param name="others">校验包含的对象</param>
		/// <returns>完全被包含返回 true</returns>
		bool isOtherXDirAttributeValuesIncludeThisXDirAttributeValues( const std::vector< HtmlString_Shared > &others ) const {
			return isLeftXDirAttributeValuesIncludeRightXDirAttributeValues( others, *this->values.get( ) );
		}

		/// <summary>
		/// 列表是否包含本对象的所有值<br/>
		/// 列表可以不存在对象的值，但是对象的所有值必须存在列表当中
		/// </summary>
		/// <param name="others">校验包含的对象</param>
		/// <returns>完全被包含返回 true</returns>
		bool isOtherXDirAttributeValuesIncludeThisXDirAttributeValues( const XDirAttribute &others ) const {
			return isOtherXDirAttributeValuesIncludeThisXDirAttributeValues( *others.values );
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
		static Vector_HtmlStringSPtr_Shared converXDirAttributeValues( const HtmlChar *buff, const size_t buff_size, const HtmlString_Shared &name );
		/// <summary>
		/// 追加一个属性
		/// </summary>
		/// <param name="save_vector_xdirattribute_s">属性列表</param>
		/// <param name="name_shared">属性名称</param>
		/// <returns>失败返回非 0</returns>
		static int32_t appendAttribute( Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s, const HtmlString_Shared &name_shared );
		/// <summary>
		/// 追加一个属性
		/// </summary>
		/// <param name="save_vector_xdirattribute_s">属性列表</param>
		/// <param name="name_shared">属性名称</param>
		/// <param name="values_vector_shared">属性值列表</param>
		/// <returns>失败返回非 0</returns>
		static int32_t appendAttribute( Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s, const HtmlString_Shared &name_shared, const Vector_HtmlStringSPtr_Shared &values_vector_shared );
		/// <summary>
		/// 使一个缓冲转换为属性属性列表
		/// </summary>
		/// <param name="conver_buff">缓冲指针</param>
		/// <param name="conver_buff_size">缓冲大小</param>
		/// <returns>转换后的属性列表，失败返回 nullptr</returns>
		static Vector_HtmlStringSPtr_Shared htmlCharBuffConverToValues( const HtmlChar *conver_buff, const size_t conver_buff_size );
		/// <summary>
		/// 使一个缓冲转换为属性名称
		/// </summary>
		/// <param name="conver_buff">缓冲指针</param>
		/// <param name="conver_buff_size">缓冲大小</param>
		/// <returns>转换后的名称，失败返回 nullptr</returns>
		static HtmlString_Shared htmlCharBuffConverToName( const HtmlChar *conver_buff, const size_t conver_buff_size );
		/// <summary>
		/// 解析字符串当中的属性
		/// </summary>
		/// <param name="buff">缓冲</param>
		/// <param name="buff_size">缓冲长度</param>
		/// <param name="save_vector_xdirattribute_s">解析对象存储位置</param>
		/// <returns>解析个数</returns>
		static size_t parseXDirAttributes( const HtmlChar *buff, const size_t buff_size, Vector_XDirAttributeSPtr_Shared &save_vector_xdirattribute_s );
		/// <summary>
		/// 判定名称是否需要切分值
		/// </summary>
		/// <param name="name">判定名称</param>
		/// <returns>需要切分返回 true</returns>
		static bool isSplitAttributeName( const HtmlString &name );
	};
}

#endif // XDIRATTRIBUTE_H_H_HEAD__FILE__
